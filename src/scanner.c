/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <u8t/scanner.h>

u8t_err_t u8t_scanner_init_with_file(u8t_scanner_t* s, FILE* file) {
	return U8T_ERR_UNEXPECTED;
}

u8t_err_t u8t_scanner_init_with_str(u8t_scanner_t* s, const char* str, size_t len) {
	return U8T_ERR_UNEXPECTED;
}

u8t_err_t u8t_scanner_scan(u8t_scanner_t* s, u8t_token_t* t) {
	return U8T_OK;
}

u8t_err_t u8t_scanner_read_token(u8t_scanner_t* s, u8t_token_t* t, char* dst, size_t n) {
	return U8T_OK;
}

