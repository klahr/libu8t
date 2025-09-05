#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(Something) {
	const char* str = "Hello,\n \"世🙂界!\" 123 45.67 _varName \"string\"";
	u8t_scanner* s = u8t_scanner_new(str, strlen(str));

	char32_t t;
	size_t n;
	t = u8t_scanner_scan(s);
	ASSERT(U8T_IDENTIFIER == t, "Expected identifier");
	ASSERT_STR_EQ("Hello", u8t_scanner_token_text(s, &n), "Expected identifier 'Hello'");
	ASSERT(u8t_scanner_line(s) == 1, "Expected line 1");
	t = u8t_scanner_scan(s);
	ASSERT(U',' == t, "Expected comma");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_STRING == t, "Expected string");
	ASSERT(strcmp("世🙂界", u8t_scanner_token_text(s, &n)), "Expected string '世🙂界'");
	ASSERT(u8t_scanner_line(s) == 2, "Expected line 2");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_INTEGER == t, "Expected integer");
	ASSERT_STR_EQ("123", u8t_scanner_token_text(s, &n), "Expected integer '123'");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_FLOAT == t, "Expected float");
	ASSERT_STR_EQ("45.67", u8t_scanner_token_text(s, &n), "Expected float '45.67'");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_IDENTIFIER == t, "Expected identifier");
	ASSERT_STR_EQ("_varName", u8t_scanner_token_text(s, &n), "Expected identifier '_varName'");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_STRING == t, "Expected string");
	ASSERT_STR_EQ("string", u8t_scanner_token_text(s, &n), "Expected string 'string'");
	t = u8t_scanner_scan(s);
	ASSERT(U8T_EOF == t, "Expected EOF");

	u8t_scanner_free(s);
}

int main(void) {
	return UC_PrintResults();
}

