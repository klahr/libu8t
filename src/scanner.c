/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "utf8.h"

static bool is_digit(char32_t c) {
	return c >= U'0' && c <= U'9';
}

static bool is_identifier_start_default(char32_t c) {
	return (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z') || c == U'_';
}

bool u8t_scanner_init(u8t_scanner* s, const char* str) {
	if (!s || !str) {
		return false;
	}

	if (utf8valid((const utf8_int8_t*)str) != 0) {
		return false;
	}

	s->_str = str;
	s->_token_start = 0u;
	s->_token_len = 0u;
	s->_token_text[0] = '\0';
	s->_token_truncated = false;
	s->is_identifier_start = is_identifier_start_default;

	return true;
}

static size_t u8t_scanner_remaining(const u8t_scanner* s) {
	if (!s) {
		return 0u;
	}
	const size_t cap = (U8T_SCANNER_MAX_TOKEN_LEN > 0u) ? (U8T_SCANNER_MAX_TOKEN_LEN - 1u) : 0u;
	const size_t used = utf8len(s->_token_text);
	return (used < cap) ? (cap - used) : 0u;
}

char32_t u8t_scanner_scan(u8t_scanner* s) {
	if (!s) {
		return U8T_EOF;
	}
	memset(s->_token_text, 0, sizeof(s->_token_text));
	s->_token_start += s->_token_len;
	s->_token_len = 0u;
	s->_token_truncated = false;

	utf8_int32_t cp = 0;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
	if (cp == 0) {
		return U8T_EOF;
	}

	if (cp == U' ' || cp == U'\t' || cp == U'\r' || cp == U'\n') {
		s->_str = next;
		s->_token_text[0] = cp;
		s->_token_len = 1u;
		return u8t_scanner_scan(s);
	}

	char32_t type;
	if (cp == U'"') {
		if (!utf8catcodepoint(s->_token_text + s->_token_len, cp, u8t_scanner_remaining(s))) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		bool done = false;
		while (!done) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (peek_cp == U'"' || peek_cp == 0) {
				done = true;
			}
			if (!utf8catcodepoint(s->_token_text + s->_token_len, peek_cp, u8t_scanner_remaining(s))) {
				s->_token_truncated = true;
			}
			++s->_token_len;
			s->_str = next;
			next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
		}
		type = U8T_STRING;
	} else if (is_digit(cp) || (cp == U'-' && is_digit(u8t_scanner_peek(s)))) {
		if (!utf8catcodepoint(s->_token_text + s->_token_len, cp, u8t_scanner_remaining(s))) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		bool has_exponent = false;
		type = U8T_INTEGER;
		for (;;) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (!is_digit(peek_cp)) {
				if (peek_cp == U'.') {
					if (type == U8T_FLOAT) {
						break;
					}
					type = U8T_FLOAT;
					if (!utf8catcodepoint(s->_token_text + s->_token_len, peek_cp, u8t_scanner_remaining(s))) {
						s->_token_truncated = true;
					}
					++s->_token_len;
				} else if (peek_cp == U'e' || peek_cp == U'E') {
					if (has_exponent) {
						break;
					}
					if (!utf8catcodepoint(s->_token_text + s->_token_len, peek_cp, u8t_scanner_remaining(s))) {
						s->_token_truncated = true;
					}
					++s->_token_len;
					has_exponent = true;
				} else {
					break;
				}
			} else {
				if (!utf8catcodepoint(s->_token_text + s->_token_len, peek_cp, u8t_scanner_remaining(s))) {
					s->_token_truncated = true;
				}
				++s->_token_len;
			}
			s->_str = next;
			next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
		}
	} else if (s->is_identifier_start(cp)) {
		if (!utf8catcodepoint(s->_token_text + s->_token_len, cp, u8t_scanner_remaining(s))) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		type = U8T_IDENTIFIER;
		for (;;) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (s->is_identifier_start(peek_cp) || is_digit(peek_cp)) {
				if (!utf8catcodepoint(s->_token_text + s->_token_len, peek_cp, u8t_scanner_remaining(s))) {
					s->_token_truncated = true;
				}
				s->_str = next;
				next = (const char*)utf8codepoint((const utf8_int8_t*)next, &cp);
				++s->_token_len;
			} else {
				break;
			}
		}
	} else {
		type = cp;
		s->_token_text[0] = (char)cp;
		s->_token_text[1] = '\0';
		s->_token_len = 1u;
	}

	s->_str = next;
	return type;
}

char32_t u8t_scanner_peek(u8t_scanner* s) {
	if (!s || s->_str == NULL || *s->_str == '\0') {
		return 0;
	}
	utf8_int32_t cp;
	const char* next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
	if (!next || *next == '\0') {
		return 0;
	}
	utf8codepoint((const utf8_int8_t*)next, &cp);
	return cp;
}

const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n) {
	if (!s) {
		if (n) {
			*n = 0u;
		}
		return NULL;
	}
	if (n) {
		*n = utf8len(s->_token_text);
	}
	return s->_token_text;
}

size_t u8t_scanner_token_start(u8t_scanner* s) {
	if (!s) {
		return 0u;
	}
	return s->_token_start;
}

size_t u8t_scanner_token_len(u8t_scanner* s) {
	if (!s) {
		return 0u;
	}
	return s->_token_len;
}

bool u8t_scanner_token_truncated(u8t_scanner* s) {
	if (!s) {
		return false;
	}
	return s->_token_truncated;
}

