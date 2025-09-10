/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include <uchar.h>
#include <stddef.h>
#include <stdbool.h>

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

typedef struct u8t_scanner {
	const char* _str;
	size_t _token_start;
	size_t _token_len;
	bool _token_truncated;
	bool (*is_identifier_start)(char32_t c);
	char _token_text[U8T_SCANNER_MAX_TOKEN_LEN];
} u8t_scanner;

bool u8t_scanner_init(u8t_scanner* s, const char* str);
char32_t u8t_scanner_scan(u8t_scanner* s);
char32_t u8t_scanner_peek(u8t_scanner* s);
const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n);
size_t u8t_scanner_token_start(u8t_scanner* s);
size_t u8t_scanner_token_len(u8t_scanner* s);
bool u8t_scanner_token_truncated(u8t_scanner* s);

#ifdef __cplusplus
}
#endif

#endif

