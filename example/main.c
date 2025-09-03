#include <u8t/scanner.h>
#include <string.h>

int main(void) {
	const char* src = "int main() { return 0; }";

	u8t_scanner_t scanner;
	if (u8t_scanner_init(&scanner, src, strlen(src)) != 0) {
		return 1;
	}

	while (1) {
		char t = u8t_scanner_scan(&scanner);
		if (t == 0) {
			break;
		}

		switch (t) {
			case U8T_INTEGER:
			case U8T_FLOAT:
			case U8T_STRING:
			default:
				break;
		}
		size_t n;
		printf("Token type: %c, text: '%s'\n", t, u8t_scanner_token_text(&scanner, &n));
	}

	return 0;
}

