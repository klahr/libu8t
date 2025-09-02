/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_TOKEN_H
#define U8T_TOKEN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct token_t {
	int type;
	char* text;
	size_t text_len;
} token_t;

void token_init(token_t* token, int type, const char* dst, size_t n);

#ifdef __cplusplus
}
#endif

#endif

