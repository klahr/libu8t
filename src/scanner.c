/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>
#include <string.h>
#include "utf8.h"

u8t_err_t u8t_scanner_init(u8t_scanner_t* s, const char* str, size_t len) {
	if (!s || !str || len == 0) {
		return U8T_ERR_INVALID;
	}

	if (utf8valid((const utf8_int8_t*)str) != 0) {
		return U8T_ERR_INVALID;
	}

	s->str = str;
	s->len = len;
	s->cursor = 0;
	s->line = 1;
	s->offset = 0;
	s->token_text[0] = '\0';
	s->token_text_len = 0;

	return U8T_OK;
}

char32_t u8t_scanner_scan(u8t_scanner_t* s) {
	s->token_text[0] = '\0';

	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->str, &cp);
	if (cp == 0) {
		return U8T_EOF;
	}

	if (cp == U' ' || cp == U'\t' || cp == U'\r') {
		s->str = next;
		++s->cursor;
		++s->offset;
		return u8t_scanner_scan(s);
	}

	if (cp == U'\n') {
		s->str = next;
		++s->cursor;
		s->offset = 0;
		++s->line;
		return u8t_scanner_scan(s);
	}

	char32_t type;
	if (cp == U'"') {
		int done = 0;
		while (!done) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (peek_cp == U'"' || peek_cp == 0) {
				done = 1;
			} else {
				utf8cat(s->token_text, (utf8_int8_t*)&peek_cp);
			}
			s->str = next;
			next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
		}
		type = U8T_STRING;
	} else {
		type = cp;
	}

	s->str = next;
	++s->cursor;
	++s->offset;
	return type;
}

char32_t u8t_scanner_peek(u8t_scanner_t* s) {
	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->str, &cp);
	utf8codepoint((const utf8_int8_t*)next, &cp);
	return cp;
}

const char* u8t_scanner_token_text(u8t_scanner_t* s, size_t* n) {
	*n = strlen(s->token_text);
	return s->token_text;
}

