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

static bool is_hex_digit(char32_t c) {
	return is_digit(c) || (c >= U'a' && c <= U'f') || (c >= U'A' && c <= U'F');
}

static bool is_binary_digit(char32_t c) {
	return c == U'0' || c == U'1';
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
	s->_token_text_pos = 0u;
	s->_token_text[0] = '\0';
	s->_token_truncated = false;
	s->is_identifier_start = is_identifier_start_default;

	return true;
}

static size_t u8t_scanner_remaining(const u8t_scanner* s) {
	if (!s) {
		return 0u;
	}
	// Use tracked byte position instead of utf8len() for O(1) instead of O(n)
	const size_t cap = (U8T_SCANNER_MAX_TOKEN_LEN > 0u) ? (U8T_SCANNER_MAX_TOKEN_LEN - 1u) : 0u;
	return (s->_token_text_pos < cap) ? (cap - s->_token_text_pos) : 0u;
}

// Helper: append codepoint to token buffer and update position tracker
// Returns true on success, false if truncated
static bool scanner_append_cp(u8t_scanner* s, char32_t cp) {
	if (s->_token_truncated) {
		return false;
	}
	size_t remaining = u8t_scanner_remaining(s);
	if (remaining == 0) {
		return false;
	}
	utf8_int8_t* result = utf8catcodepoint(
		(utf8_int8_t*)(s->_token_text + s->_token_text_pos), cp, remaining);
	if (!result) {
		return false;
	}
	// Update position based on bytes written
	s->_token_text_pos = (size_t)(result - (utf8_int8_t*)s->_token_text);
	// Null-terminate (utf8catcodepoint doesn't do this)
	s->_token_text[s->_token_text_pos] = '\0';
	return true;
}

static void scanner_take_cp(u8t_scanner* s, char32_t peek_cp, const char** next, utf8_int32_t* cp) {
	if (!scanner_append_cp(s, peek_cp)) {
		s->_token_truncated = true;
	}
	++s->_token_len;
	*next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, cp);
	s->_str = *next;
}

static bool scanner_scan_radix_literal(u8t_scanner* s, char32_t* result) {
	char32_t peek_cp = u8t_scanner_peek(s);
	const bool is_hex = (peek_cp == U'x' || peek_cp == U'X');
	const bool is_bin = (peek_cp == U'b' || peek_cp == U'B');
	if (!is_hex && !is_bin) {
		return false;
	}

	utf8_int32_t cp;
	const char* next;
	if (!scanner_append_cp(s, peek_cp)) {
		s->_token_truncated = true;
	}
	++s->_token_len;
	next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
	s->_str = next;

	bool has_digits = false;
	for (;;) {
		peek_cp = u8t_scanner_peek(s);
		if (!(is_hex ? is_hex_digit(peek_cp) : is_binary_digit(peek_cp))) {
			break;
		}
		has_digits = true;
		scanner_take_cp(s, peek_cp, &next, &cp);
	}

	*result = has_digits ? (char32_t)U8T_INTEGER : (char32_t)U8T_ERROR;
	return true;
}

static char32_t scanner_scan_number_body(u8t_scanner* s, char32_t first_digit, const char** next, utf8_int32_t* cp) {
	// Check for hex (0x) or binary (0b) prefix
	if (first_digit == U'0') {
		char32_t radix_result;
		if (scanner_scan_radix_literal(s, &radix_result)) {
			*next = s->_str;
			return radix_result;
		}
	}

	bool has_exponent = false;
	bool has_fraction = false;
	bool malformed = false;
	char32_t type = U8T_INTEGER;

	for (;;) {
		char32_t peek_cp = u8t_scanner_peek(s);
		if (is_digit(peek_cp)) {
			scanner_take_cp(s, peek_cp, next, cp);
			continue;
		}
		if (peek_cp == U'.') {
			if (has_fraction) {
				break;
			}
			has_fraction = true;
			type = U8T_FLOAT;
			scanner_take_cp(s, peek_cp, next, cp);
			continue;
		}
		if (peek_cp == U'e' || peek_cp == U'E') {
			if (has_exponent) {
				break;
			}
			has_exponent = true;
			scanner_take_cp(s, peek_cp, next, cp);
			// The sign of an exponent is part of the number; a sign anywhere else ends it.
			peek_cp = u8t_scanner_peek(s);
			if (peek_cp == U'+' || peek_cp == U'-') {
				scanner_take_cp(s, peek_cp, next, cp);
				peek_cp = u8t_scanner_peek(s);
			}
			if (is_digit(peek_cp)) {
				type = U8T_FLOAT;
			} else {
				malformed = true;
			}
			continue;
		}
		break;
	}

	return malformed ? (char32_t)U8T_ERROR : type;
}

char32_t u8t_scanner_scan(u8t_scanner* s) {
	if (!s) {
		return U8T_EOF;
	}
	// Reset token buffer - just null-terminate at start instead of clearing all 2048 bytes
	s->_token_text[0] = '\0';
	s->_token_text_pos = 0u;
	s->_token_start += s->_token_len;
	s->_token_len = 0u;
	s->_token_truncated = false;

	utf8_int32_t cp = 0;
	const char* next;
	for (;;) {
		next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
		if (cp == 0) {
			return U8T_EOF;
		}
		if (cp != U' ' && cp != U'\t' && cp != U'\r' && cp != U'\n') {
			break;
		}
		s->_str = next;
		++s->_token_start;
	}

	char32_t type;
	if (cp == U'"') {
		if (!scanner_append_cp(s, cp)) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		s->_str = next;
		bool done = false;
		bool escaped = false;
		while (!done) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (peek_cp == 0) {
				break;
			}
			if (escaped) {
				escaped = false;
			} else if (peek_cp == U'\\') {
				escaped = true;
			} else if (peek_cp == U'"') {
				done = true;
			}
			if (!scanner_append_cp(s, peek_cp)) {
				s->_token_truncated = true;
			}
			++s->_token_len;
			next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
			s->_str = next;
		}
		type = U8T_STRING;
	} else if (is_digit(cp)) {
		if (!scanner_append_cp(s, cp)) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		s->_str = next;
		type = scanner_scan_number_body(s, cp, &next, &cp);
	} else if (s->is_identifier_start(cp)) {
		if (!scanner_append_cp(s, cp)) {
			s->_token_truncated = true;
		}
		++s->_token_len;
		type = U8T_IDENTIFIER;
		s->_str = next;
		for (;;) {
			char32_t peek_cp = u8t_scanner_peek(s);
			if (s->is_identifier_start(peek_cp) || is_digit(peek_cp)) {
				if (!scanner_append_cp(s, peek_cp)) {
					s->_token_truncated = true;
				}
				next = (const char*)utf8codepoint((const utf8_int8_t*)s->_str, &cp);
				s->_str = next;
				++s->_token_len;
			} else {
				break;
			}
		}
	} else if (cp == U'-') {
		// Check if minus is followed by a digit for negative numbers
		utf8_int32_t next_cp;
		utf8codepoint((const utf8_int8_t*)next, &next_cp);
		if (is_digit(next_cp)) {
			// Negative number
			if (!scanner_append_cp(s, cp)) {
				s->_token_truncated = true;
			}
			++s->_token_len;
			s->_str = next;

			// Consume the leading digit, which puts the scanner in the state the positive
			// path above reaches, so the rest of the number is one shared path.
			scanner_take_cp(s, next_cp, &next, &cp);
			type = scanner_scan_number_body(s, next_cp, &next, &cp);
		} else {
			// Standalone minus
			type = cp;
			if (!scanner_append_cp(s, cp)) {
				s->_token_truncated = true;
			}
			s->_token_len = 1u;
		}
	} else {
		type = cp;
		if (!scanner_append_cp(s, cp)) {
			s->_token_truncated = true;
		}
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
	utf8codepoint((const utf8_int8_t*)s->_str, &cp);
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

