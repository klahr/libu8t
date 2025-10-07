#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(UnderscoreVariations) {
	u8t_scanner s;
	u8t_scanner_init(&s, "_ _a a_ _123 __double__");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Single underscore");
	ASSERT_STR_EQ("_", u8t_scanner_token_text(&s, &n), "Should be _");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Underscore prefix");
	ASSERT_STR_EQ("_a", u8t_scanner_token_text(&s, &n), "Should be _a");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Underscore suffix");
	ASSERT_STR_EQ("a_", u8t_scanner_token_text(&s, &n), "Should be a_");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Underscore with digits");
	ASSERT_STR_EQ("_123", u8t_scanner_token_text(&s, &n), "Should be _123");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Double underscore");
	ASSERT_STR_EQ("__double__", u8t_scanner_token_text(&s, &n), "Should be __double__");
}

TEST(MixedCase) {
	u8t_scanner s;
	u8t_scanner_init(&s, "CamelCase SCREAMING_SNAKE snake_case");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "CamelCase");
	ASSERT_STR_EQ("CamelCase", u8t_scanner_token_text(&s, &n), "Should be CamelCase");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "SCREAMING_SNAKE");
	ASSERT_STR_EQ("SCREAMING_SNAKE", u8t_scanner_token_text(&s, &n), "Should be SCREAMING_SNAKE");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "snake_case");
	ASSERT_STR_EQ("snake_case", u8t_scanner_token_text(&s, &n), "Should be snake_case");
}

TEST(UTF8_Identifiers) {
	u8t_scanner s;
	u8t_scanner_init(&s, "hello 世界 42");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "ASCII identifier");
	ASSERT_STR_EQ("hello", u8t_scanner_token_text(&s, &n), "Should be hello");

	t = u8t_scanner_scan(&s);

	while ((t = u8t_scanner_scan(&s)) != U8T_EOF && t != U8T_INTEGER) {
		continue;
	}
	ASSERT_EQ(U8T_INTEGER, t, "Should find integer");
	ASSERT_STR_EQ("42", u8t_scanner_token_text(&s, &n), "Should be 42");
}

TEST(RepeatedScanning) {
	u8t_scanner s;
	u8t_scanner_init(&s, "a b c");

	u8t_scanner_scan(&s);
	u8t_scanner_scan(&s);
	u8t_scanner_scan(&s);

	for (int i = 0; i < 5; i++) {
		char32_t t = u8t_scanner_scan(&s);
		ASSERT_EQ(U8T_EOF, t, "Repeated scans after EOF should return EOF");
	}
}

TEST(FloatsWithMultipleDecimalsInSequence) {
	u8t_scanner s;
	u8t_scanner_init(&s, "1.2.3.4.5");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "First float");
	ASSERT_STR_EQ("1.2", u8t_scanner_token_text(&s, &n), "Should be 1.2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Decimal");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Second float");
	ASSERT_STR_EQ("3.4", u8t_scanner_token_text(&s, &n), "Should be 3.4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Decimal");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Integer");
	ASSERT_STR_EQ("5", u8t_scanner_token_text(&s, &n), "Should be 5");
}

