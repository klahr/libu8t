/* SPDX-License-Identifier: GPL-3.0-or-later */

/*! \file scanner.h
	\brief A simple UTF-8 scanner for tokenizing strings.
	\author klahr
	\date 2025-09-10
*/
#ifndef U8T_SCANNER_H
#define U8T_SCANNER_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/* FreeBSD's <uchar.h> defines char8_t as a typedef, which conflicts with
 * C++20's built-in char8_t type. We work around this by:
 * - In C++20+: char8_t is built-in, char32_t is in <cuchar>
 * - In C/older C++: include <uchar.h> for char32_t
 */
#if defined(__cplusplus) && __cplusplus >= 202002L
#include <cuchar>
#else
#include <uchar.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! Maximum token (literal) length (including null terminator).
#ifndef U8T_SCANNER_MAX_TOKEN_LEN
#define U8T_SCANNER_MAX_TOKEN_LEN 2048
#endif

//! Token types.
enum {
	U8T_EOF = 0,
	U8T_IDENTIFIER,
	U8T_INTEGER,
	U8T_FLOAT,
	U8T_STRING,
};

//! The scanner structure.
/*!
	This structure holds the state of the scanner.
*/
typedef struct u8t_scanner {
	const char* _str;
	size_t _token_start;
	size_t _token_len;
	bool _token_truncated;
	bool (*is_identifier_start)(char32_t c);
	char _token_text[U8T_SCANNER_MAX_TOKEN_LEN];
} u8t_scanner;

/*! \fn bool u8t_scanner_init(u8t_scanner* s, const char* str)
	\param s Pointer to the scanner structure.
	\param str Pointer to the UTF-8 string to scan.
	\return true on success, false on failure.
	\note The string must be null-terminated.
	\note The string must remain valid until the scanner is no longer used.
*/
bool u8t_scanner_init(u8t_scanner* s, const char* str);

/*! \fn char32_t u8t_scanner_scan(u8t_scanner* s)
	\param s Pointer to the scanner structure.
	\return The next token type, or U8T_EOF if the end of the string is reached.
*/
char32_t u8t_scanner_scan(u8t_scanner* s);

/*! \fn char32_t u8t_scanner_peek(u8t_scanner* s)
	\param s Pointer to the scanner structure.
	\return The next character that will be scanned without advancing the scanner position.
	\note Returns 0 if the end of the string is reached or if there is no next character.
	\note This peeks at the character after the current scanning position.
*/
char32_t u8t_scanner_peek(u8t_scanner* s);

/*! \fn const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n)
	\param s Pointer to the scanner structure.
	\param n Pointer to a size_t variable to receive the length of the token text.
	\return Pointer to the token text.
	\note The returned string is null-terminated.
	\note The returned string is valid until the next call to u8t_scanner_scan().
*/
const char* u8t_scanner_token_text(u8t_scanner* s, size_t* n);

/*! \fn size_t u8t_scanner_token_start(u8t_scanner* s)
	\param s Pointer to the scanner structure.
	\return The starting index of the current token in the original string.
*/
size_t u8t_scanner_token_start(u8t_scanner* s);

/*! \fn size_t u8t_scanner_token_len(u8t_scanner* s)
	\param s Pointer to the scanner structure.
	\return The length of the current token.
*/
size_t u8t_scanner_token_len(u8t_scanner* s);

/*! \fn bool u8t_scanner_token_truncated(u8t_scanner* s)
	\param s Pointer to the scanner structure.
	\return true if the current token was truncated due to exceeding the maximum length, false otherwise.
*/
bool u8t_scanner_token_truncated(u8t_scanner* s);

#ifdef __cplusplus
}
#endif

#endif

