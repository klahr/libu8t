/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include <uchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef U8T_SCANNER_MAX_TOKEN_LEN
#define U8T_SCANNER_MAX_TOKEN_LEN 2048
#endif

enum {
	U8T_EOF = 0,
	U8T_IDENTIFIER,
	U8T_INTEGER,
	U8T_FLOAT,
	U8T_STRING,
};

struct u8t_scanner;
typedef struct u8t_scanner u8t_scanner;

u8t_scanner* u8t_scanner_new(const char* str, size_t len);
void u8t_scanner_free(u8t_scanner* s);
char32_t u8t_scanner_scan(u8t_scanner* s);
char32_t u8t_scanner_peek(u8t_scanner* s);
const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n);
size_t u8t_scanner_token_start(u8t_scanner* s);
size_t u8t_scanner_token_len(u8t_scanner* s);

#ifdef __cplusplus
}
#endif

#endif

