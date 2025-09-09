#include <u8t/scanner.h>
#include <string.h>

int main(void) {
	const char* src = "int main() {\n\tconst char* hello = \"Hiö!\"; const char* x = \"\";\n\tfloat df = -12.3E4e;\n\tint x = 1234;\n\treturn 0;\n}\n";

	u8t_scanner* scanner = u8t_scanner_new(src);
	if (!scanner) {
		return 1;
	}

	while (1) {
		char32_t t = u8t_scanner_scan(scanner);
		size_t n;
		switch (t) {
			case U8T_EOF:
				printf("Found end of file: %s\n", u8t_scanner_token_text(scanner, &n));
				break;
			case U8T_INTEGER:
				printf("int   : %s\n", u8t_scanner_token_text(scanner, &n));
				break;
			case U8T_FLOAT:
				printf("float : %s\n", u8t_scanner_token_text(scanner, &n));
				break;
			case U8T_STRING:
				printf("string: %s\n", u8t_scanner_token_text(scanner, &n));
				break;
			case U8T_IDENTIFIER:
				printf("ident : %s\n", u8t_scanner_token_text(scanner, &n));
				break;
			default:
				printf("token : %c\n", t);
				break;
		}

		if (t == 0) {
			break;
		}

	}

	u8t_scanner_free(scanner);

	return 0;
}

