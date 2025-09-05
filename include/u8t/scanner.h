/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include <uchar.h>
#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	U8T_EOF = 0,
	U8T_IDENTIFIER,
	U8T_INTEGER,
	U8T_FLOAT,
	U8T_STRING,
};

typedef struct u8t_scanner {
	const char* str;
	size_t len;
	size_t cursor;
	size_t line;
	size_t offset;
	char token_text[256];
	int (*is_identifier_start)(char32_t c);
} u8t_scanner;

u8t_scanner* u8t_scanner_new(const char* str, size_t len);
void u8t_scanner_free(u8t_scanner* s);
char32_t u8t_scanner_scan(u8t_scanner* s);
char32_t u8t_scanner_peek(u8t_scanner* s);
const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n);
size_t u8t_scanner_line(u8t_scanner* s);
size_t u8t_scanner_offset(u8t_scanner* s);

#ifdef __cplusplus
}
#endif

#endif

