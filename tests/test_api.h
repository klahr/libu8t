#include <unit-check/uc.h>
#include <u8t/scanner.h>

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
	ASSERT_EQ(U'a', peek, "Peek should return 'a' (current character)");

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

TEST(AllAPIFunctionsWithNull) {
	ASSERT_EQ(U8T_EOF, u8t_scanner_scan(NULL), "scan(NULL) returns EOF");
	ASSERT_EQ(0, u8t_scanner_peek(NULL), "peek(NULL) returns 0");
	ASSERT_EQ(NULL, u8t_scanner_token_text(NULL, NULL), "token_text(NULL) returns NULL");
	ASSERT_EQ(0, u8t_scanner_token_start(NULL), "token_start(NULL) returns 0");
	ASSERT_EQ(0, u8t_scanner_token_len(NULL), "token_len(NULL) returns 0");
	ASSERT_EQ(false, u8t_scanner_token_truncated(NULL), "token_truncated(NULL) returns false");
}

