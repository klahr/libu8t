/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scanner_t {
	char* whitespaces;
	const char* str;
	size_t len;
	size_t cursor;
	size_t token_text_len;
	char token_text[256];
} u8t_scanner_t;

u8t_err_t u8t_scanner_init(u8t_scanner_t* s, const char* str, size_t len);
char u8t_scanner_scan(u8t_scanner_t* s);
const char* u8t_scanner_token_text(u8t_scanner_t* s, size_t* n);

#ifdef __cplusplus
}
#endif

#endif

