#include <u8t/err.h>

const char* u8t_err_str(u8t_err_t err) {
	switch (err) {
	case U8T_ERR_OK:
		return "No error";
	case U8T_ERR_UNEXPECTED:
		return "Unexpected error";
	default:
		return "Unknown error";
	}
}

