/**
 * @brief Project for Wireless and Mobile Networks course on BUT FIT
 * @author Fridolin Pokorny
 * @date 10 Oct 2013
 */

#ifndef CODER_H_
#define CODER_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <cstdlib>
#include <vector>

extern "C" {
    #include "lib/ecc.h"
}

enum ret_t {
    RET_OK = 0,
    RET_HELP,
    RET_ERR_FILE_IN,
    RET_ERR_FILE_OUT,
    RET_ERR_SECURE,
    RET_ERR_RECOVER,
    RET_ERR_ARGS,
};

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

#define DATA_BLOCK_LEN  100
#define UPPER_BLOCK_LEN 4096
#define BLOCK_LEN       (DATA_BLOCK_LEN + NPAR)

#endif // CODER_H_

