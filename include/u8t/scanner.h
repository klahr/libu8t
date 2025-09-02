/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scanner_t {
	char* whitespaces;
} scanner_t;

void scanner_init_with_file(scanner_t* s, FILE* file);
void scanner_init_with_str(scanner_t* s, const char* str, size_t len);
int scanner_scan(scanner_t* s, token_t* t);
int scanner_read_token(scanner_t* s, token_t* t, char* dst, size_t n);

#ifdef __cplusplus
}
#endif

#endif

