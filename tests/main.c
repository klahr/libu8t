#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(Something) {
	const char* str = "Hello,\n \"世🙂界!\" 123 45.67 _varName \"string\"";
	u8t_scanner* s = u8t_scanner_new(str, strlen(str));

	char32_t t;
	size_t n;
	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("Hello", u8t_scanner_token_text(s, &n), "Expected identifier 'Hello'");
	ASSERT_EQ(u8t_scanner_token_start(s), 0, "Expected start 0");
	ASSERT_EQ(u8t_scanner_token_len(s), 5, "Expected len 5");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U',', t, "Expected comma");
	ASSERT_EQ(u8t_scanner_token_start(s), 5, "Expected start 5");
	ASSERT_EQ(u8t_scanner_token_len(s), 1, "Expected len 1");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT_EQ(u8t_scanner_token_start(s), 8, "Expected start 9");
	ASSERT_EQ(u8t_scanner_token_len(s), 6, "Expected len 4");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_INTEGER, t, "Expected integer");
	ASSERT_STR_EQ("123", u8t_scanner_token_text(s, &n), "Expected integer '123'");
	ASSERT_EQ(u8t_scanner_token_start(s), 15, "Expected start 15");
	ASSERT_EQ(u8t_scanner_token_len(s), 3, "Expected len 3");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_FLOAT, t, "Expected float");
	ASSERT_STR_EQ("45.67", u8t_scanner_token_text(s, &n), "Expected float '45.67'");
	ASSERT_EQ(u8t_scanner_token_start(s), 19, "Expected start 19");
	ASSERT_EQ(u8t_scanner_token_len(s), 5, "Expected len 5");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("_varName", u8t_scanner_token_text(s, &n), "Expected identifier '_varName'");
	ASSERT_EQ(u8t_scanner_token_start(s), 25, "Expected start 25");
	ASSERT_EQ(u8t_scanner_token_len(s), 8, "Expected len 8");

	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT(strcmp("\"string\"", u8t_scanner_token_text(s, &n)) == 0, "Expected string 'string'");
	ASSERT_EQ(u8t_scanner_token_start(s), 34, "Expected start 25");
	ASSERT_EQ(u8t_scanner_token_len(s), 8, "Expected len 8");
	t = u8t_scanner_scan(s);
	ASSERT_EQ(U8T_EOF, t, "Expected EOF");

	u8t_scanner_free(s);
}

int main(void) {
	return UC_PrintResults();
}

