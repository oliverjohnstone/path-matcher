//
// Created by Oliver Johnstone on 10/04/2020.
//

#include <gtest/gtest.h>
#include <string>
#include "../src/lexer.h"

using namespace PathMatcher;
using namespace std;

namespace {
    TEST(Lexer, ItParsesExpectedTokens) {
        string str = "/test/:arg/(.*)/:arg2";
        Lexer lexer(str);
        LexToken expected[] = {
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
            {.index=(int)str.length() + 1, .value="", .type=LexTokenType::END}
        };

        auto tokens = lexer.getTokens();

        EXPECT_EQ(tokens.size(), 12);

        for (int i = 0; i < tokens.size(); i++) {
            auto tok = tokens[i];
            auto expectedTok = expected[i];

            EXPECT_EQ(tok.index, expectedTok.index);
            EXPECT_STREQ(tok.value.c_str(), expectedTok.value.c_str());
            EXPECT_EQ(tok.type, expectedTok.type);
        }
    }

    TEST(Lexer, ItThrowsAnExceptionWithInvalidNameArg) {
        ASSERT_THROW({
            Lexer lexer("/:");
        }, invalid_argument);
    }

    TEST(Lexer, ItThrowsAnExceptionWithEmptyCaptureGroup) {
        ASSERT_THROW({
            Lexer lexer("/()");
        }, invalid_argument);
    }

    TEST(Lexer, ItThrowsAnExceptionWithInvalidCaptureGroup) {
        ASSERT_THROW({
            Lexer lexer("/(?.*)");
        }, invalid_argument);
    }

    TEST(Lexer, ItThrowsAnExceptionWithNestedCaptureGroup) {
        ASSERT_THROW({
            Lexer lexer("/(.*(?.*))");
        }, invalid_argument);
    }

    TEST(Lexer, ItThrowsAnExceptionWithUnbalancedCaptureGroup) {
        ASSERT_THROW({
            Lexer lexer("/(((((.*))))");
        }, invalid_argument);
    }
}