/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include "token.h"
#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scanner_t {
	char* whitespaces;
} u8t_scanner_t;

u8t_err_t u8t_scanner_init_with_file(u8t_scanner_t* s, FILE* file);
u8t_err_t u8t_scanner_init_with_str(u8t_scanner_t* s, const char* str, size_t len);
u8t_err_t u8t_scanner_scan(u8t_scanner_t* s, u8t_token_t* t);
u8t_err_t u8t_scanner_read_token(u8t_scanner_t* s, u8t_token_t* t, char* dst, size_t n);

#ifdef __cplusplus
}
#endif

#endif

