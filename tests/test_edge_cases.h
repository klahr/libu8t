#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(WhitespaceOnly) {
	u8t_scanner s;
	u8t_scanner_init(&s, "   \t\n\r  ");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Whitespace only should return EOF");
}

TEST(SpecialCharacters) {
	u8t_scanner s;
	u8t_scanner_init(&s, "(){}[];,.:!@#$%^&*+-=<>?/\\|");

	char32_t t;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'(', t, "Open paren");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U')', t, "Close paren");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'{', t, "Open brace");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'}', t, "Close brace");

	while ((t = u8t_scanner_scan(&s)) != U8T_EOF) {
		ASSERT(t != U8T_IDENTIFIER && t != U8T_INTEGER && t != U8T_FLOAT && t != U8T_STRING,
		       "Should be special character token");
	}
}

TEST(ControlCharactersAreNotTokenTypes) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\001\002\003\004");

	char32_t t;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(0x0001, t, "U+0001 is returned as its own codepoint");
	ASSERT(t != U8T_IDENTIFIER, "U+0001 must not be mistaken for an identifier");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(0x0002, t, "U+0002 is returned as its own codepoint");
	ASSERT(t != U8T_INTEGER, "U+0002 must not be mistaken for an integer");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(0x0003, t, "U+0003 is returned as its own codepoint");
	ASSERT(t != U8T_FLOAT, "U+0003 must not be mistaken for a float");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(0x0004, t, "U+0004 is returned as its own codepoint");
	ASSERT(t != U8T_STRING, "U+0004 must not be mistaken for a string");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "End of input");
}

TEST(TokenTypesAreOutsideTheCodepointRange) {
	ASSERT(U8T_IDENTIFIER > 0x1FFFFF, "U8T_IDENTIFIER is above any decodable codepoint");
	ASSERT(U8T_INTEGER > 0x1FFFFF, "U8T_INTEGER is above any decodable codepoint");
	ASSERT(U8T_FLOAT > 0x1FFFFF, "U8T_FLOAT is above any decodable codepoint");
	ASSERT(U8T_STRING > 0x1FFFFF, "U8T_STRING is above any decodable codepoint");

	u8t_scanner s;
	u8t_scanner_init(&s, "\xF7\xBF\xBF\xBF");
	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(0x1FFFFF, t, "Highest decodable value comes back as a codepoint");
	ASSERT(t != U8T_IDENTIFIER && t != U8T_INTEGER && t != U8T_FLOAT && t != U8T_STRING,
	       "A codepoint must never collide with a token type");
}

TEST(NumbersStartingWithDecimal) {
	u8t_scanner s;
	u8t_scanner_init(&s, ".5 .123");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Decimal point alone is special char");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "5 is integer");
	ASSERT_STR_EQ("5", u8t_scanner_token_text(&s, &n), "Should be 5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Second decimal point");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "123 is integer");
}

TEST(IdentifiersWithNumbers) {
	u8t_scanner s;
	u8t_scanner_init(&s, "var1 test_2 foo3bar hello123world");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "var1");
	ASSERT_STR_EQ("var1", u8t_scanner_token_text(&s, &n), "Should be var1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "test_2");
	ASSERT_STR_EQ("test_2", u8t_scanner_token_text(&s, &n), "Should be test_2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "foo3bar");
	ASSERT_STR_EQ("foo3bar", u8t_scanner_token_text(&s, &n), "Should be foo3bar");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "hello123world");
	ASSERT_STR_EQ("hello123world", u8t_scanner_token_text(&s, &n), "Should be hello123world");
}

