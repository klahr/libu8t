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
	ASSERT_EQ(u8t_scanner_token_start(&s), 0, "Expected start 0");
	ASSERT_EQ(u8t_scanner_token_len(&s), 5, "Expected len 5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U',', t, "Expected comma");
	ASSERT_EQ(u8t_scanner_token_start(&s), 5, "Expected start 5");
	ASSERT_EQ(u8t_scanner_token_len(&s), 1, "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT_EQ(u8t_scanner_token_start(&s), 8, "Expected start 9");
	ASSERT_EQ(u8t_scanner_token_len(&s), 6, "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Expected integer");
	ASSERT_STR_EQ("123", u8t_scanner_token_text(&s, &n), "Expected integer '123'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 15, "Expected start 15");
	ASSERT_EQ(u8t_scanner_token_len(&s), 3, "Expected len 3");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_FLOAT, t, "Expected float");
	ASSERT_STR_EQ("45.67", u8t_scanner_token_text(&s, &n), "Expected float '45.67'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 19, "Expected start 19");
	ASSERT_EQ(u8t_scanner_token_len(&s), 5, "Expected len 5");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("_varName", u8t_scanner_token_text(&s, &n), "Expected identifier '_varName'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 25, "Expected start 25");
	ASSERT_EQ(u8t_scanner_token_len(&s), 8, "Expected len 8");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT(strcmp("\"string\"", u8t_scanner_token_text(&s, &n)) == 0, "Expected string 'string'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 34, "Expected start 25");
	ASSERT_EQ(u8t_scanner_token_len(&s), 8, "Expected len 8");

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
	ASSERT_EQ(u8t_scanner_token_start(&s), 0, "Expected start 0");
	ASSERT_EQ(u8t_scanner_token_len(&s), 2, "Expected len 2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected identifier");
	ASSERT_STR_EQ("main", u8t_scanner_token_text(&s, &n), "Expected identifier 'main'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 3, "Expected start 3");
	ASSERT_EQ(u8t_scanner_token_len(&s), 4, "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'(', t, "Expected (");
	ASSERT_EQ(u8t_scanner_token_start(&s), 7, "Expected start 7");
	ASSERT_EQ(u8t_scanner_token_len(&s), 1, "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U')', t, "Expected )");
	ASSERT_EQ(u8t_scanner_token_start(&s), 8, "Expected start 8");
	ASSERT_EQ(u8t_scanner_token_len(&s), 1, "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'{', t, "Expected {");
	ASSERT_EQ(u8t_scanner_token_start(&s), 10, "Expected start 8");
	ASSERT_EQ(u8t_scanner_token_len(&s), 1, "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected push");
	ASSERT_STR_EQ("push", u8t_scanner_token_text(&s, &n), "Expected identifier 'push'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 13, "Expected start 13");
	ASSERT_EQ(u8t_scanner_token_len(&s), 4, "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Expected -8");
	ASSERT_STR_EQ("-8", u8t_scanner_token_text(&s, &n), "Expected integer '-8'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 18, "Expected start 18");
	ASSERT_EQ(u8t_scanner_token_len(&s), 2, "Expected len 2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Expected push");
	ASSERT_STR_EQ("push", u8t_scanner_token_text(&s, &n), "Expected identifier 'push'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 22, "Expected start 22");
	ASSERT_EQ(u8t_scanner_token_len(&s), 4, "Expected len 4");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Expected string");
	ASSERT(strcmp("\"Hello, world!\"", u8t_scanner_token_text(&s, &n)) == 0, "Expected string 'Hello, world!'");
	ASSERT_EQ(u8t_scanner_token_start(&s), 27, "Expected start 27");
	ASSERT_EQ(u8t_scanner_token_len(&s), 15, "Expected len 15");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U'}', t, "Expected }");
	ASSERT_EQ(u8t_scanner_token_start(&s), 43, "Expected start 43");
	ASSERT_EQ(u8t_scanner_token_len(&s), 1, "Expected len 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Expected EOF");
}

TEST(EmptyString) {
	u8t_scanner s;
	u8t_scanner_init(&s, "");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Empty string should return EOF");
}

TEST(WhitespaceOnly) {
	u8t_scanner s;
	u8t_scanner_init(&s, "   \t\n\r  ");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_EOF, t, "Whitespace only should return EOF");
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

TEST(NullPointerHandling) {
	bool result = u8t_scanner_init(NULL, "test");
	ASSERT_EQ(false, result, "NULL scanner should return false");

	u8t_scanner s;
	result = u8t_scanner_init(&s, NULL);
	ASSERT_EQ(false, result, "NULL string should return false");

	char32_t t = u8t_scanner_scan(NULL);
	ASSERT_EQ(U8T_EOF, t, "NULL scanner should return EOF");

	size_t n;
	const char* text = u8t_scanner_token_text(NULL, &n);
	ASSERT_EQ(NULL, text, "NULL scanner should return NULL text");
}

TEST(SingleCharacterTokens) {
	u8t_scanner s;
	u8t_scanner_init(&s, "a b c 1 2 3");

	char32_t t;
	size_t n;

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Single char identifier");
	ASSERT_STR_EQ("a", u8t_scanner_token_text(&s, &n), "Should be a");
	ASSERT_EQ(1, n, "Length should be 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Single char identifier");
	ASSERT_STR_EQ("b", u8t_scanner_token_text(&s, &n), "Should be b");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Single char identifier");
	ASSERT_STR_EQ("c", u8t_scanner_token_text(&s, &n), "Should be c");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Single digit");
	ASSERT_STR_EQ("1", u8t_scanner_token_text(&s, &n), "Should be 1");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Single digit");
	ASSERT_STR_EQ("2", u8t_scanner_token_text(&s, &n), "Should be 2");

	t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_INTEGER, t, "Single digit");
	ASSERT_STR_EQ("3", u8t_scanner_token_text(&s, &n), "Should be 3");
}

TEST(TokenPositions) {
	u8t_scanner s;
	u8t_scanner_init(&s, "abc 123");

	u8t_scanner_scan(&s);
	ASSERT_EQ(0, u8t_scanner_token_start(&s), "First token starts at 0");
	ASSERT_EQ(3, u8t_scanner_token_len(&s), "First token length is 3");

	u8t_scanner_scan(&s);
	ASSERT_EQ(4, u8t_scanner_token_start(&s), "Second token starts at 4");
	ASSERT_EQ(3, u8t_scanner_token_len(&s), "Second token length is 3");
}

TEST(PeekFunction) {
	u8t_scanner s;
	u8t_scanner_init(&s, "abc");

	char32_t peek = u8t_scanner_peek(&s);
	ASSERT_EQ(U'b', peek, "Peek should return 'b' (next after 'a')");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_IDENTIFIER, t, "Should scan identifier");

	u8t_scanner_init(&s, "a");
	u8t_scanner_scan(&s);
	peek = u8t_scanner_peek(&s);
	ASSERT_EQ(0, peek, "Peek at EOF should return 0");

	peek = u8t_scanner_peek(NULL);
	ASSERT_EQ(0, peek, "Peek on NULL should return 0");
}

TEST(TokenTruncation) {
	u8t_scanner s;

	char long_str[3000];
	for (int i = 0; i < 2999; i++) {
		long_str[i] = 'a';
	}
	long_str[2999] = '\0';

	u8t_scanner_init(&s, long_str);
	u8t_scanner_scan(&s);

	ASSERT_EQ(true, u8t_scanner_token_truncated(&s), "Long token should be truncated");

	size_t n;
	u8t_scanner_token_text(&s, &n);
	ASSERT(n < 2999, "Token length should be less than input");

	u8t_scanner_init(&s, "short");
	u8t_scanner_scan(&s);
	ASSERT_EQ(false, u8t_scanner_token_truncated(&s), "Short token should not be truncated");
}

TEST(LongStrings) {
	u8t_scanner s;

	char long_str[3000];
	long_str[0] = '"';
	for (int i = 1; i < 2998; i++) {
		long_str[i] = 'x';
	}
	long_str[2998] = '"';
	long_str[2999] = '\0';

	u8t_scanner_init(&s, long_str);
	char32_t t = u8t_scanner_scan(&s);

	ASSERT_EQ(U8T_STRING, t, "Should recognize as string");
	ASSERT_EQ(true, u8t_scanner_token_truncated(&s), "Long string should be truncated");
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

TEST(MixedWhitespace) {
	u8t_scanner s;
	u8t_scanner_init(&s, "a\t\t\tb\n\n\nc\r\n\r\nd");

	size_t n;

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("a", u8t_scanner_token_text(&s, &n), "First identifier");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("b", u8t_scanner_token_text(&s, &n), "Second identifier");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("c", u8t_scanner_token_text(&s, &n), "Third identifier");

	u8t_scanner_scan(&s);
	ASSERT_STR_EQ("d", u8t_scanner_token_text(&s, &n), "Fourth identifier");
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

TEST(StringsWithNewlines) {
	u8t_scanner s;
	u8t_scanner_init(&s, "\"hello\nworld\"");

	char32_t t = u8t_scanner_scan(&s);
	ASSERT_EQ(U8T_STRING, t, "Multi-line string");

	size_t n;
	const char* text = u8t_scanner_token_text(&s, &n);
	ASSERT(text != NULL, "Token text should not be NULL");
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

TEST(TokenTextWithNullLength) {
	u8t_scanner s;
	u8t_scanner_init(&s, "hello");

	u8t_scanner_scan(&s);

	const char* text = u8t_scanner_token_text(&s, NULL);
	ASSERT(text != NULL, "Should return text even with NULL length param");
}

TEST(AllAPIFunctionsWithNull) {
	ASSERT_EQ(U8T_EOF, u8t_scanner_scan(NULL), "scan(NULL) returns EOF");
	ASSERT_EQ(0, u8t_scanner_peek(NULL), "peek(NULL) returns 0");
	ASSERT_EQ(NULL, u8t_scanner_token_text(NULL, NULL), "token_text(NULL) returns NULL");
	ASSERT_EQ(0, u8t_scanner_token_start(NULL), "token_start(NULL) returns 0");
	ASSERT_EQ(0, u8t_scanner_token_len(NULL), "token_len(NULL) returns 0");
	ASSERT_EQ(false, u8t_scanner_token_truncated(NULL), "token_truncated(NULL) returns false");
}

int main(void) {
	return UC_PrintResults();
}

