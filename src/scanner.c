/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>

void scanner_init_with_file(scanner_t* s, FILE* file) {
}

void scanner_init_with_str(scanner_t* s, const char* str, size_t len) {
}

int scanner_scan(scanner_t* s, token_t* t) {
	return 0;
}

int scanner_read_token(scanner_t* s, token_t* t, char* dst, size_t n) {
	return 0;
}

