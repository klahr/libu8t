#include <u8t/scanner.h>

int main(void) {
	const char* src = "int main() { return 0; }";

	u8t_scanner_t scanner;
	if (u8t_scanner_init_with_str(&scanner, src, 20) != 0) {
		return 1;
	}

	char buf[256] = { 0 };
	u8t_token_t token = {
		.text = buf,
		.text_len = sizeof(buf)
	};

	while (u8t_scanner_scan(&scanner, &token) == 0) {
		printf("Token type: %d, text: '%s'\n", token.type, token.text);
	}

	return 0;
}

