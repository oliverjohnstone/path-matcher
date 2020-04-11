//
// Created by Oliver Johnstone on 10/04/2020.
//

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../src/parser.h"

using namespace PathMatcher;
using namespace std;

namespace {
    TEST(Parser, ParsesLexTokensIntoParseTokens) {
        vector<LexToken> lexTokens = {
            {.index=0, .value="/", .type=LexTokenType::CHAR},
            {.index=1, .value="t", .type=LexTokenType::CHAR},
            {.index=2, .value="e", .type=LexTokenType::CHAR},
            {.index=3, .value="s", .type=LexTokenType::CHAR},
            {.index=4, .value="t", .type=LexTokenType::CHAR},
            {.index=5, .value="/", .type=LexTokenType::CHAR},
            {.index=6, .value="arg", .type=LexTokenType::NAME},
            {.index=10, .value="/", .type=LexTokenType::CHAR},
            {.index=11, .value=".*", .type=LexTokenType::PATTERN},
            {.index=15, .value="/", .type=LexTokenType::CHAR},
            {.index=16, .value="arg2", .type=LexTokenType::NAME},
            {.index=21, .value="", .type=LexTokenType::END}
        };
        Parser parser(lexTokens);

        const auto& parseTokens = parser.getTokens();

        EXPECT_EQ(parseTokens.size(), 4);

        const auto& test = get<string>(parseTokens[0]);
        const auto& arg = get<ParseToken>(parseTokens[1]);
        const auto& pattern = get<ParseToken>(parseTokens[2]);
        const auto& arg2 = get<ParseToken>(parseTokens[3]);

        // Test string variant
        EXPECT_STREQ(test.c_str(), "/test");

        // Test Arg ParseToken variant
        EXPECT_STREQ(get<string>(arg.name).c_str(), "arg");
        EXPECT_STREQ(arg.prefix.c_str(), "/");
        EXPECT_STREQ(arg.suffix.c_str(), "");
        EXPECT_STREQ(arg.modifier.c_str(), "");
        EXPECT_STREQ(arg.pattern.c_str(), R"([.\w_\-~]+?)");

        // Pattern arg ParseToken variant
        EXPECT_EQ(get<int>(pattern.name), 0);
        EXPECT_STREQ(pattern.prefix.c_str(), "/");
        EXPECT_STREQ(pattern.suffix.c_str(), "");
        EXPECT_STREQ(pattern.modifier.c_str(), "");
        EXPECT_STREQ(pattern.pattern.c_str(), R"(.*)");

        // Arg2 ParseToken variant
        EXPECT_STREQ(get<string>(arg2.name).c_str(), "arg2");
        EXPECT_STREQ(arg2.prefix.c_str(), "/");
        EXPECT_STREQ(arg2.suffix.c_str(), "");
        EXPECT_STREQ(arg2.modifier.c_str(), "");
        EXPECT_STREQ(arg2.pattern.c_str(), R"([.\w_\-~]+?)");
    }

    TEST(Parser, ItThrowsAnExceptionWhenMissingRequiredTokens) {
        vector<LexToken> lexTokens = {
            {.index=0, .value="(", .type=LexTokenType::OPEN},
            {.index=1, .value="", .type=LexTokenType::END}
        };

        EXPECT_THROW({
            Parser p(lexTokens);
        }, invalid_argument);
    }
}