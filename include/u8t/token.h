/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_TOKEN_H
#define U8T_TOKEN_H

#include "err.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct u8t_token_t {
	int type;
	char* text;
	size_t text_len;
} u8t_token_t;

u8t_err_t u8t_token_init(u8t_token_t* token, int type, const char* dst, size_t n);

#ifdef __cplusplus
}
#endif

#endif

