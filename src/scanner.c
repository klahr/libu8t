/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>
#include <string.h>
#include "utf8.h"

typedef struct u8t_scanner {
	const char* str;
	size_t len;
	size_t token_start;
	size_t token_len;
	int token_truncated;
	int (*is_identifier_start)(char32_t c);
	char token_text[U8T_SCANNER_MAX_TOKEN_LEN];
} u8t_scanner;

int is_digit(char32_t c) {
	return c >= U'0' && c <= U'9';
}

int is_identifier_start(char32_t c) {
	return (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z') || c == U'_';
}

u8t_scanner* u8t_scanner_new(const char* str, size_t len) {
	if (!str || len == 0) {
		return NULL;
	}

	if (utf8valid((const utf8_int8_t*)str) != 0) {
		return NULL;
	}

	u8t_scanner* s = (u8t_scanner*)malloc(sizeof(u8t_scanner));
	if (!s) {
		return NULL;
	}

	s->str = str;
	s->len = len;
	s->token_start = 0;
	s->token_len = 0;
	s->token_text[0] = '\0';
	s->token_truncated = 0;
	s->is_identifier_start = is_identifier_start;

	return s;
}

void u8t_scanner_free(u8t_scanner* s) {
	if (s) {
		free(s);
	}
}

size_t u8t_scanner_remaining(u8t_scanner* s) {
	if (!s) {
		return 0;
	}
	int remaining = U8T_SCANNER_MAX_TOKEN_LEN - 1 - s->token_len;
	return remaining > 0 ? remaining : 0;
}

char32_t u8t_scanner_scan(u8t_scanner* s) {
	memset(s->token_text, 0, sizeof(s->token_text));
	s->token_start += s->token_len;
	s->token_len = 0;
	s->token_truncated = 0;

	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->str, &cp);
	if (cp == 0) {
		return U8T_EOF;
	}

	if (cp == U' ' || cp == U'\t' || cp == U'\r') {
		s->str = next;
		s->token_text[0] = cp;
		s->token_len = 1;
		return u8t_scanner_scan(s);
	}

	if (cp == U'\n') {
		s->str = next;
		s->token_text[0] = '\n';
		s->token_len = 1;
		return u8t_scanner_scan(s);
	}

	char32_t type;
	if (cp == U'"') {
		if (!utf8catcodepoint(s->token_text + s->token_len, cp, u8t_scanner_remaining(s))) {
			s->token_truncated = 1;
		}
		++s->token_len;
		int done = 0;
		while (!done) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (peek_cp == U'"' || peek_cp == 0) {
				done = 1;
			}
		if (!utf8catcodepoint(s->token_text + s->token_len, peek_cp, u8t_scanner_remaining(s))) {
			s->token_truncated = 1;
		}
			++s->token_len;
			s->str = next;
			next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
		}
		type = U8T_STRING;
	} else if (is_digit(cp) || (cp == U'-' && is_digit(u8t_scanner_peek(s)))) {
		if (!utf8catcodepoint(s->token_text + s->token_len, cp, u8t_scanner_remaining(s))) {
			s->token_truncated = 1;
		}
		++s->token_len;
		int has_exponent = 0;
		type = U8T_INTEGER;
		for (;;) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (!is_digit(peek_cp)) {
				if (peek_cp == U'.') {
					if (type == U8T_FLOAT) {
						break;
					}
					type = U8T_FLOAT;
					if (!utf8catcodepoint(s->token_text + s->token_len, peek_cp, u8t_scanner_remaining(s))) {
						s->token_truncated = 1;
					}
					++s->token_len;
				} else if (peek_cp == U'e' || peek_cp == U'E') {
					if (has_exponent) {
						break;
					}
					if (!utf8catcodepoint(s->token_text + s->token_len, peek_cp, u8t_scanner_remaining(s))) {
						s->token_truncated = 1;
					}
					++s->token_len;
					has_exponent = 1;
				} else {
					break;
				}
			} else {
				if (!utf8catcodepoint(s->token_text + s->token_len, peek_cp, u8t_scanner_remaining(s))) {
					s->token_truncated = 1;
				}
				++s->token_len;
			}
			s->str = next;
			next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
		}
	} else if (s->is_identifier_start(cp)) {
		if (!utf8catcodepoint(s->token_text + s->token_len, cp, u8t_scanner_remaining(s))) {
			s->token_truncated = 1;
		}
		++s->token_len;
		int done = 0;
		type = U8T_IDENTIFIER;
		while (!done) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (s->is_identifier_start(peek_cp) || is_digit(peek_cp)) {
				if (!utf8catcodepoint(s->token_text + s->token_len, peek_cp, u8t_scanner_remaining(s))) {
					s->token_truncated = 1;
				}
				s->str = next;
				next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
				++s->token_len;
			} else {
				done = 1;
			}
		}
	} else {
		type = cp;
		s->token_text[0] = (char)cp;
		s->token_text[1] = '\0';
		s->token_len = 1;
	}

	s->str = next;
	return type;
}

char32_t u8t_scanner_peek(u8t_scanner* s) {
	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->str, &cp);
	utf8codepoint((const utf8_int8_t*)next, &cp);
	return cp;
}

const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n) {
	*n = strlen(s->token_text);
	return s->token_text;
}

size_t u8t_scanner_token_start(u8t_scanner* s) {
	if (!s) {
		return 0;
	}
	return s->token_start;
}

size_t u8t_scanner_token_len(u8t_scanner* s) {
	if (!s) {
		return 0;
	}
	return s->token_len;
}

