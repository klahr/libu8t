#include <unit-check/uc.h>
#include <u8t/scanner.h>

TEST(NegativeNumbers) {
	u8t_scanner s;
	u8t_scanner_init(&s, "-42 -3.14 -0");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Negative integer");
	ASSERT_STR_EQ("-42", u8t_scanner_token_text(&s, &n), "Should be -42");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Negative float");
	ASSERT_STR_EQ("-3.14", u8t_scanner_token_text(&s, &n), "Should be -3.14");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Negative zero");
	ASSERT_STR_EQ("-0", u8t_scanner_token_text(&s, &n), "Should be -0");
}

TEST(ScientificNotation) {
	u8t_scanner s;
	u8t_scanner_init(&s, "1e5 2.5E-3 3.14e+10 6.022E+23");

	size_t n;

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("1e5", u8t_scanner_token_text(&s, &n), "Scientific notation 1e5");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("2.5E-3", u8t_scanner_token_text(&s, &n), "Scientific notation 2.5E-3");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("3.14e+10", u8t_scanner_token_text(&s, &n), "Scientific notation 3.14e+10");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("6.022E+23", u8t_scanner_token_text(&s, &n), "Scientific notation 6.022E+23 (Avogadro's number)");
}

TEST(ScientificNotation_EdgeCases) {
	u8t_scanner s;
	size_t n;
	char32_t t;

	u8t_scanner_init(&s, "1e");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "1e without digits treated as integer");

	u8t_scanner_init(&s, "1e+");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "1e+ without digits treated as integer");

	u8t_scanner_init(&s, "1E-0");
	t = u8t_scanner_scan(&s);
	ASSERT_STR_EQ("1E-0", u8t_scanner_token_text(&s, &n), "Zero exponent");

	u8t_scanner_init(&s, "-1.5e-10");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Negative with negative exponent");
	ASSERT_STR_EQ("-1.5e-10", u8t_scanner_token_text(&s, &n), "Should be -1.5e-10");

	u8t_scanner_init(&s, "1e5e3");
	t = u8t_scanner_scan(&s);
	ASSERT_STR_EQ("1e5", u8t_scanner_token_text(&s, &n), "Only first exponent");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "e3 becomes identifier");

	u8t_scanner_init(&s, "1e308");
	t = u8t_scanner_scan(&s);
	ASSERT_STR_EQ("1e308", u8t_scanner_token_text(&s, &n), "Large exponent");
}

TEST(LeadingZeros) {
	u8t_scanner s;
	u8t_scanner_init(&s, "007 0.5 00.00");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Integer with leading zeros");
	ASSERT_STR_EQ("007", u8t_scanner_token_text(&s, &n), "Should be 007");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Float starting with 0");
	ASSERT_STR_EQ("0.5", u8t_scanner_token_text(&s, &n), "Should be 0.5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Float with leading zeros");
	ASSERT_STR_EQ("00.00", u8t_scanner_token_text(&s, &n), "Should be 00.00");
}

TEST(MultipleDecimalPoints) {
	u8t_scanner s;
	u8t_scanner_init(&s, "3.14.159");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "First part should be float");
	ASSERT_STR_EQ("3.14", u8t_scanner_token_text(&s, &n), "Should stop at second decimal");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Second decimal should be separate token");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Remaining should be integer");
	ASSERT_STR_EQ("159", u8t_scanner_token_text(&s, &n), "Should be 159");
}

TEST(MinusWithoutDigit) {
	u8t_scanner s;
	u8t_scanner_init(&s, "- 5");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'-', t, "Minus should be separate token");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Number should follow");
	ASSERT_STR_EQ("5", u8t_scanner_token_text(&s, &n), "Should be 5");
}

TEST(StringsWithNewlines) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"hello\nworld\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Multi-line string");

	size_t n;
	const char* text = u8t_scanner_token_text(&s, &n);
	ASSERT(text != NULL, "Token text should not be NULL");
}

TEST(TokenTextWithNullLength) {
	u8t_scanner s;
	u8t_scanner_init(&s, "hello");

	u8t_scanner_scan(&s);

	const char* text = u8t_scanner_token_text(&s, NULL);
	ASSERT(text != NULL, "Should return text even with NULL length param");
}

