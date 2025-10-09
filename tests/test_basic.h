#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(Source) {
	const char* str = "Hello,\n \"世🙂界!\" 123 45.67 _varName \"string\"";
	u8t_scanner s;
	u8t_scanner_init(&s, str);

	char32_t t;
	size_t n;
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("Hello", u8t_scanner_token_text(&s, &n), "Expected identifier 'Hello'");
	ASSERT_EQ(0, u8t_scanner_token_start(&s), "Expected start 0");
	ASSERT_EQ(5, u8t_scanner_token_len(&s), "Expected len 5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U',', t, "Expected comma");
	ASSERT_EQ(5, u8t_scanner_token_start(&s), "Expected start 5");
	ASSERT_EQ(1, u8t_scanner_token_len(&s), "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT_EQ(8, u8t_scanner_token_start(&s), "Expected start 9");
	ASSERT_EQ(6, u8t_scanner_token_len(&s), "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Expected integer");
	ASSERT_STR_EQ("123", u8t_scanner_token_text(&s, &n), "Expected integer '123'");
	ASSERT_EQ(15, u8t_scanner_token_start(&s), "Expected start 15");
	ASSERT_EQ(3, u8t_scanner_token_len(&s), "Expected len 3");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Expected float");
	ASSERT_STR_EQ("45.67", u8t_scanner_token_text(&s, &n), "Expected float '45.67'");
	ASSERT_EQ(19, u8t_scanner_token_start(&s), "Expected start 19");
	ASSERT_EQ(5, u8t_scanner_token_len(&s), "Expected len 5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("_varName", u8t_scanner_token_text(&s, &n), "Expected identifier '_varName'");
	ASSERT_EQ(25, u8t_scanner_token_start(&s), "Expected start 25");
	ASSERT_EQ(8, u8t_scanner_token_len(&s), "Expected len 8");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT(strcmp("\"string\"", u8t_scanner_token_text(&s, &n)) == 0, "Expected string 'string'");
	ASSERT_EQ(34, u8t_scanner_token_start(&s), "Expected start 25");
	ASSERT_EQ(8, u8t_scanner_token_len(&s), "Expected len 8");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Expected EOF");
}

TEST(Scanner) {
	const char* str = "fn main() {\n\tpush -8\n\tpush \"Hello, world!\"\n}";
	u8t_scanner s;
	u8t_scanner_init(&s, str);

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("fn", u8t_scanner_token_text(&s, &n), "Expected identifier 'fn'");
	ASSERT_EQ(0, u8t_scanner_token_start(&s), "Expected start 0");
	ASSERT_EQ(2, u8t_scanner_token_len(&s), "Expected len 2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("main", u8t_scanner_token_text(&s, &n), "Expected identifier 'main'");
	ASSERT_EQ(3, u8t_scanner_token_start(&s), "Expected start 3");
	ASSERT_EQ(4, u8t_scanner_token_len(&s), "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'(', t, "Expected (");
	ASSERT_EQ(7, u8t_scanner_token_start(&s), "Expected start 7");
	ASSERT_EQ(1, u8t_scanner_token_len(&s), "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U')', t, "Expected )");
	ASSERT_EQ(8, u8t_scanner_token_start(&s), "Expected start 8");
	ASSERT_EQ(1, u8t_scanner_token_len(&s), "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'{', t, "Expected {");
	ASSERT_EQ(10, u8t_scanner_token_start(&s), "Expected start 8");
	ASSERT_EQ(1, u8t_scanner_token_len(&s), "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected push");
	ASSERT_STR_EQ("push", u8t_scanner_token_text(&s, &n), "Expected identifier 'push'");
	ASSERT_EQ(13, u8t_scanner_token_start(&s), "Expected start 13");
	ASSERT_EQ(4, u8t_scanner_token_len(&s), "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Expected -8");
	ASSERT_STR_EQ("-8", u8t_scanner_token_text(&s, &n), "Expected integer '-8'");
	ASSERT_EQ(18, u8t_scanner_token_start(&s), "Expected start 18");
	ASSERT_EQ(2, u8t_scanner_token_len(&s), "Expected len 2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected push");
	ASSERT_STR_EQ("push", u8t_scanner_token_text(&s, &n), "Expected identifier 'push'");
	ASSERT_EQ(22, u8t_scanner_token_start(&s), "Expected start 22");
	ASSERT_EQ(4, u8t_scanner_token_len(&s), "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT(strcmp("\"Hello, world!\"", u8t_scanner_token_text(&s, &n)) == 0, "Expected string 'Hello, world!'");
	ASSERT_EQ(27, u8t_scanner_token_start(&s), "Expected start 27");
	ASSERT_EQ(15, u8t_scanner_token_len(&s), "Expected len 15");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'}', t, "Expected }");
	ASSERT_EQ(43, u8t_scanner_token_start(&s), "Expected start 43");
	ASSERT_EQ(1, u8t_scanner_token_len(&s), "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Expected EOF");
}

