/* SPDX-License-Identifier: GPL-3.0-or-later */

#ifndef U8T_ERR_H
#define U8T_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	U8T_OK = 0,
	U8T_ERR_INVALID,
	U8T_ERR_UNEXPECTED,
} u8t_err;

const char* u8t_err_str(u8t_err err);

#ifdef __cplusplus
}
#endif

#endif

