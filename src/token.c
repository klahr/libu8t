/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/token.h>

u8t_err_t u8t_token_init(u8t_token_t* token, char* dst, size_t n) {
	if (!token || !dst || n == 0) {
		return U8T_ERR_INVALID;
	}

	token->type = 0;
	token->text = (char*)dst;
	token->text_len = n;

	return U8T_OK;
}

