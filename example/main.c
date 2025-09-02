#include <u8t/scanner.h>

int main(void) {
	const char* src = "int main() { return 0; }";

	u8t_scanner_t scanner;
	if (u8t_scanner_init_with_str(&scanner, src, 20) != 0) {
		return 1;
	}

	char buf[256] = { 0 };
	u8t_token_t token;
	if (u8t_token_init(&token, buf, sizeof(buf)) != U8T_OK) {
		return 1;
	}

	while (u8t_scanner_scan(&scanner, &token) == U8T_OK) {
		printf("Token type: %d, text: '%s'\n", token.type, token.text);
	}

	return 0;
}

