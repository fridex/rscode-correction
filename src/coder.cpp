/**
 * @brief Project for Wireless and Mobile Networks course on BUT FIT
 * @author Fridolin Pokorny
 * @date 31 Oct 2013
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <cstdlib>

#include "coder.h"

#define AUTHOR "Fridolin Pokorny <fridex.devel@gmail.com>"

// Using common main, determinate which executable is going to be built...
#ifdef BMS1B
  #define STR_SUFFIX ".ok"
  #define PROG_DESC  "Recover given file and try to fix bit errors.\n"
  #define DO_STUFF recover_file
#else
#ifdef BMS1A
  #define STR_SUFFIX ".rsecc"
  #define PROG_DESC  "Secure given file for bit errors.\n"
  #define DO_STUFF secure_file
#else
  #error "Macro DO_STUFF not defined!"
#endif // BMS1A
#endif // BMS1B

/**
 */
#define DATA_BLOCK_LEN		100
#define BLOCK_LEN		(DATA_BLOCK_LEN + NPAR)

ret_t secure_file(std::ofstream & file_out, std::vector<char> & data);
ret_t recover_file(std::ofstream & file_out, std::vector<char> & data);

/*******************************************************************************/

/* Introduce a byte error at LOC */
void
byte_err(int err, int loc, char *dst) {
    //printf("Adding Error at loc %d, data %#x\n", loc, dst[loc-1]);
    dst[loc - 1] ^= err;
}

/*******************************************************************************/

/**
 * Print simple help
 * @param pname name of the executable
 * @return RET_HELP
 */
static int print_help(const char * pname) {
    using namespace std;

    cout << PROG_DESC
         << "USAGE: " << pname << " FILE\n\n"
         << "\tFILE\t- input file\n"
         << "\t-h\t- print this help\n\n"
         << "Written by " << AUTHOR << endl;

    return RET_HELP;
}

/**
 * Make interleaving - encode
 *
 * @param out output array
 * @param data input array
 */
void shuffle_secure(std::vector<char> & out, std::vector<char> & data) {
    /*
     * interleave using matrix - number of columns == blocks
     * data are written in reverse order as follows, consider block size 5,
     * matrix will look like this:
     *    1 2 3 4 5
     *    6 7 8 9 A
     *    B C
     * data written to file are:
     *    1 6 B 2 7 C 3 8 4 9 5 A
     */
    size_t k = 0;
    for (int i = 0; i < BLOCK_LEN; ++i) {
        size_t j = 0;

        while (i + j * BLOCK_LEN < data.size()) {
            out[k++] = data[i + j * BLOCK_LEN];
            j++;
        }
    }
}

/**
 * Make interleaving - decode
 *
 * @param out output array
 * @param data input array
 */
void shuffle_recover(std::vector<char> & out, std::vector<char> & data) {
    /*
     * Data from shuffle_recover should be placed in correct order, consider a
     * matrix from shuffle_secure comment
     * There is an error propagation when data are not aligned to matrix
     * correctly (there are some spare spaces). This makes an error in indexing,
     * which has to be substracted.
     *   Index of "4" is data[line*block_size + column - 1]
     *   Index of "5" is data[line*block_size + column - 2]
     */
    // error marker
    size_t err;
    if (data.size() % BLOCK_LEN == 0)
        err = 0;
    else
        err = BLOCK_LEN - (data.size() % BLOCK_LEN) - 1;

    // number of lines in matrix
    size_t lines = data.size() / BLOCK_LEN;
    if (lines * BLOCK_LEN < data.size())
        lines++;

    size_t k = 0;

    for (size_t i = 0; i < lines; ++i) {
        size_t local_err = 1;

        for (size_t j = 0; j < BLOCK_LEN; ++j) {
            if (BLOCK_LEN - j <= err) {
                if (k < data.size()) {
                    out[k++] = data[i + j * lines - local_err];
                    local_err++;
                }
            } else {
                if (k < data.size()) {
                    out[k++] = data[i + j * lines];
                }
            }
        }
    }
}

#ifdef BMS1B
/**
 * Recover given file
 *
 * @param fname input file name
 * @return RET_OK on success, otherwise error code
 */
ret_t recover_file(std::ofstream & file_out, std::vector<char> & in) {
    initialize_ecc();

    size_t start = 0;
    size_t len;

    // interleave
    std::vector<char> data;
    data = in;
    shuffle_recover(data, in);

    std::vector<char> out;
    while (start < data.size()) {
        if (start + BLOCK_LEN > data.size())
            len = data.size() - start;
        else
            len = BLOCK_LEN;

        if (len <= NPAR) {
            std::cerr << "Error: file size does not match secured file size\n";
            return RET_ERR_RECOVER;
        }

        decode_data(reinterpret_cast<unsigned char *>(&data[start]), len);

        if (check_syndrome() != 0)
            correct_errors_erasures (reinterpret_cast<unsigned char *>(&data[start]), len, 0, 0);

        for (size_t i = 0; i < len - NPAR; ++i)
            out.push_back(data[start + i]);

        start += BLOCK_LEN;
    }

    file_out.write(&out[0], out.size());

    return RET_OK;
}
#endif // BMS1B

#ifdef BMS1A
/**
 * Secure given file
 *
 * @param fname input file name
 * @return RET_OK on success, otherwise error code
 */
ret_t secure_file(std::ofstream & file_out, std::vector<char> & data) {
    std::vector<char> codeword;

    initialize_ecc();

    // add space for parity
    {
        // add space for last block too
        size_t par_size = data.size() / DATA_BLOCK_LEN;
        if (par_size * DATA_BLOCK_LEN < data.size())
            par_size++;

        par_size *= NPAR; // number of bytes
        // are we after block?
        codeword.resize(data.size() + par_size, 0);
    }


    size_t start_enc = 0;
    size_t start_data = 0;
    size_t len = 0;

    while (start_data < data.size()) {
        if (start_data + DATA_BLOCK_LEN > data.size())
            len = data.size() - start_data;
        else
            len = DATA_BLOCK_LEN;

        encode_data(reinterpret_cast<unsigned char *>(&data[start_data]),
                    len,
                    reinterpret_cast<unsigned char *>(&codeword[start_enc]));

        start_data += len;
        start_enc += len + NPAR;
    }

    // interleave
    std::vector<char> out;

    out = codeword;
    shuffle_secure(out, codeword);

    file_out.write(reinterpret_cast<char *>(&out[0]), out.size());

    return RET_OK;
}
#endif // BMS1A

/**
 * App entry point
 * @param argc argument count
 * @param argv argument vector
 */
int main(int argc, char * argv[]) {
    std::ifstream file_in;
    std::ofstream file_out;
    std::string fname;
    int ret;

    if (argc != 2 || ! strcmp(argv[1], "-h"))
        return print_help(argv[0]);

    fname = argv[1];

    // input file
    file_in.open(fname.c_str(), std::ios::in | std::ios::binary);
    if (! file_in.is_open()) {
        std::cerr << fname.c_str() << ":"
                  << strerror(errno) << std::endl;
        return RET_ERR_FILE_IN;
    }

    std::vector<char> data((std::istreambuf_iterator<char>(file_in)),
                            std::istreambuf_iterator<char>());
    file_in.close();

    // output file
    file_out.open((fname + STR_SUFFIX).c_str(), std::ios::out | std::ios::binary);
    if (! file_out.is_open()) {
        std::cerr << (fname + STR_SUFFIX).c_str() << ":"
                  << strerror(errno) << std::endl;
        file_in.close();
        return RET_ERR_FILE_OUT;
    }

    // now work for us!
    ret = DO_STUFF(file_out, data);

    // clear the kitchen...
    file_out.close();

    return ret;
}

