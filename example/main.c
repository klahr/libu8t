#include <u8t/scanner.h>
#include <string.h>

int main(void) {
	const char* src = "int main() {\n\tconst char* hello = \"Hiö!\"; const char* x = \"\";\n\treturn 0;\n}\n";

	u8t_scanner_t scanner;
	if (u8t_scanner_init(&scanner, src, strlen(src)) != 0) {
		return 1;
	}

	while (1) {
		char32_t t = u8t_scanner_scan(&scanner);
		size_t n;
		switch (t) {
			case U8T_EOF:
				printf("Found end of file: %s\n", u8t_scanner_token_text(&scanner, &n));
				break;
			case U8T_INTEGER:
				printf("Found an integer: %s\n", u8t_scanner_token_text(&scanner, &n));
				break;
			case U8T_FLOAT:
				printf("Found a float: %s\n", u8t_scanner_token_text(&scanner, &n));
				break;
			case U8T_STRING:
				printf("string: %s\n", u8t_scanner_token_text(&scanner, &n));
				break;
			default:
				printf("token : %c, text: '%s' %zu:%zu\n", t, u8t_scanner_token_text(&scanner, &n), scanner.line, scanner.offset);
				break;
		}

		if (t == 0) {
			break;
		}

	}

	return 0;
}

