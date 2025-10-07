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

