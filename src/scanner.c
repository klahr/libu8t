/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>

u8t_err_t u8t_scanner_init(u8t_scanner_t* s, const char* str, size_t len) {
	if (!s || !str || len == 0) {
		return U8T_ERR_INVALID;
	}

	s->whitespaces = " \t\n\r";
	s->str = str;
	s->len = len;
	s->cursor = 0;
	s->token_text[0] = '\0';
	s->token_text_len = 0;

	return U8T_OK;
}

char u8t_scanner_scan(u8t_scanner_t* s) {
	return 'A';
}

const char* u8t_scanner_token_text(u8t_scanner_t* s, size_t* n) {
	*n = 0;
	return NULL;
}

