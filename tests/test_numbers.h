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
	ASSERT_EQ(U8T_ERROR, t, "1e without exponent digits is malformed");

	u8t_scanner_init(&s, "1e+");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "1e+ without exponent digits is malformed");

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

TEST(ScientificNotationIsFloat) {
	u8t_scanner s;
	char32_t t;

	u8t_scanner_init(&s, "1e5");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "1e5 is a float");

	u8t_scanner_init(&s, "2E10");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "2E10 is a float");

	u8t_scanner_init(&s, "1E-0");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "1E-0 is a float");

	u8t_scanner_init(&s, "-1e5");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "-1e5 is a float");

	u8t_scanner_init(&s, "-2E+10");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "-2E+10 is a float");

	u8t_scanner_init(&s, "1e");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "1e has no exponent digits");

	u8t_scanner_init(&s, "1e+");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "1e+ has no exponent digits");

	u8t_scanner_init(&s, "0x1e5");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Hex literals are never floats");
}

TEST(ExponentDoesNotAffectSplitting) {
	u8t_scanner s;
	char32_t t;
	size_t n;

	u8t_scanner_init(&s, "0E11.0");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "0E11.0 is a float");
	ASSERT_STR_EQ("0E11.0", u8t_scanner_token_text(&s, &n), "0E11.0 stays a single token");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Nothing should be left over");

	u8t_scanner_init(&s, "1e5.5.5");
	t = u8t_scanner_scan(&s);
	ASSERT_STR_EQ("1e5.5", u8t_scanner_token_text(&s, &n), "Stops at the second decimal point");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'.', t, "Second decimal point is its own token");
}

TEST(IncompleteLiteralsAreErrors) {
	u8t_scanner s;
	char32_t t;
	size_t n;

	static const char* malformed[] = { "0x", "0X", "0b", "0B", "1e", "1e+", "1e-" };
	for (size_t i = 0u; i < sizeof(malformed) / sizeof(*malformed); i++) {
		u8t_scanner_init(&s, malformed[i]);
		t = u8t_scanner_scan(&s);
		ASSERT_EQ(U8T_ERROR, t, "Incomplete literal is an error");
		ASSERT_STR_EQ(malformed[i], u8t_scanner_token_text(&s, &n),
		              "Error token keeps the scanned text");
		t = u8t_scanner_scan(&s);
		ASSERT_EQ(U8T_EOF, t, "Nothing left after the error token");
	}

	u8t_scanner_init(&s, "0xFF");
	ASSERT_EQ(U8T_INTEGER, u8t_scanner_scan(&s), "0xFF is a valid hex integer");
	u8t_scanner_init(&s, "0b1010");
	ASSERT_EQ(U8T_INTEGER, u8t_scanner_scan(&s), "0b1010 is a valid binary integer");
	u8t_scanner_init(&s, "1e5");
	ASSERT_EQ(U8T_FLOAT, u8t_scanner_scan(&s), "1e5 is a valid float");
	u8t_scanner_init(&s, "0");
	ASSERT_EQ(U8T_INTEGER, u8t_scanner_scan(&s), "A bare 0 is still an integer");
	u8t_scanner_init(&s, "0x1e5");
	ASSERT_EQ(U8T_INTEGER, u8t_scanner_scan(&s), "e inside a hex literal is a digit");

	u8t_scanner_init(&s, "0xZ");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "0x with a non-hex character is an error");
	ASSERT_STR_EQ("0x", u8t_scanner_token_text(&s, &n), "Error token is just the prefix");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Scanning continues after the error token");

	u8t_scanner_init(&s, "1e.5");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "An exponent with no digits stays an error");

	u8t_scanner_init(&s, "-1e");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_ERROR, t, "-1e is malformed");
	u8t_scanner_init(&s, "-1e5");
	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "-1e5 is a valid float");
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

