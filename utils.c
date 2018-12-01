#include "utils.h"

/**
 *
 * convert string to uint16_t
 *
 * @param str
 * @param n
 * @return
 */

bool str_to_uint16(const char *str, uint16_t *n) {
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *n = (uint16_t) val;
    return true;
}

/**
 *  wrapper
 *
 * @param msg
 */
void error(const char *msg) {
    perror(msg);
    exit(1);
}

