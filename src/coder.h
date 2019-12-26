/**
 * @brief Project for Wireless and Mobile Networks course on BUT FIT
 * @author Fridolin Pokorny
 * @date 10 Oct 2013
 */

#ifndef COMMON_H_
#define COMMON_H_

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
};

#endif // COMMON_H_

