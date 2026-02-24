#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(EmptyString) {
	u8t_scanner s;
	u8t_scanner_init(&s, "");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Empty string should return EOF");
}

TEST(UnclosedString) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"unclosed string");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize unclosed string");

	size_t n;
	const char* text = u8t_scanner_token_text(&s, &n);
	ASSERT(text != NULL, "Token text should not be NULL");
}

TEST(EmptyString_Literal) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize empty string literal");

	size_t n;
	ASSERT_STR_EQ("\"\"", u8t_scanner_token_text(&s, &n), "Should be empty string");
}

TEST(ConsecutiveStrings) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"hello\"\"world\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "First string");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Second string");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "EOF");
}

TEST(EscapedQuotes) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"hello \\\"world\\\"\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize string with escaped quotes");

	size_t n;
	ASSERT_STR_EQ("\"hello \\\"world\\\"\"", u8t_scanner_token_text(&s, &n),
	              "Should contain full string with escaped quotes");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Should be EOF after string");
}

TEST(EscapedBackslashBeforeQuote) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"end\\\\\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize string with escaped backslash");

	size_t n;
	ASSERT_STR_EQ("\"end\\\\\"", u8t_scanner_token_text(&s, &n),
	              "Escaped backslash should not escape the closing quote");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Should be EOF after string");
}

TEST(CustomIdentifierStart) {
	u8t_scanner s;
	u8t_scanner_init(&s, "$var @param");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'$', t, "$ should be special char with default");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "var should be identifier");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'@', t, "@ should be special char with default");
}

TEST(EscapedQuote) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"hello \\\"world\\\"\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize string with escaped quotes");

	size_t n;
	ASSERT_STR_EQ("\"hello \\\"world\\\"\"", u8t_scanner_token_text(&s, &n),
	              "Should contain escaped quotes");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Should be EOF after string");
}

TEST(EscapedBackslash) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"path\\\\\\\\\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize string with escaped backslashes");

	size_t n;
	ASSERT_STR_EQ("\"path\\\\\\\\\"", u8t_scanner_token_text(&s, &n),
	              "Should contain escaped backslashes");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Should be EOF after string");
}

TEST(EscapedBackslashQuote) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"test\\\\\\\"end\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Should recognize string with escaped backslash then quote");

	size_t n;
	ASSERT_STR_EQ("\"test\\\\\\\"end\"", u8t_scanner_token_text(&s, &n),
	              "Should contain escaped backslash and quote");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Should be EOF after string");
}

TEST(AdjacentOperators) {
	u8t_scanner s;
	u8t_scanner_init(&s, "++--==!=<=>===");

	char32_t t;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'+', t, "First plus");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'+', t, "Second plus");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'-', t, "First minus");

	while ((t = u8t_scanner_scan(&s)) != U8T_EOF) {
		ASSERT(t != U8T_IDENTIFIER && t != U8T_INTEGER && t != U8T_FLOAT && t != U8T_STRING,
		       "Should be operator tokens");
	}
}

