#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>

bool str_to_uint16(const char *str, uint16_t *n);
void error(const char *msg);

#endif