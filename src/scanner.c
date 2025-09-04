/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>
#include "utf8.h"

u8t_err_t u8t_scanner_init(u8t_scanner_t* s, const char* str, size_t len) {
	if (!s || !str || len == 0) {
		return U8T_ERR_INVALID;
	}

	if (utf8valid((const utf8_int8_t*)str) != 0) {
		return U8T_ERR_INVALID;
	}

	s->whitespaces = " \t\n\r";
	s->str = str;
	s->len = len;
	s->cursor = 0;
	s->line = 1;
	s->col = 1;
	s->token_text[0] = '\0';
	s->token_text_len = 0;

	return U8T_OK;
}

char u8t_scanner_scan(u8t_scanner_t* s) {
	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->str, &cp);
	if (cp == 0) {
		return U8T_EOF;
	}

	if (cp == U'\n') {
		++s->line;
		s->col = 1;
	} else {
		++s->col;
	}

	char type = s->str[0];
	s->str = next;
	++s->cursor;
	return type;
}

const char* u8t_scanner_token_text(u8t_scanner_t* s, size_t* n) {
	*n = 0;
	return NULL;
}

