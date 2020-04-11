//
// Created by Oliver Johnstone on 11/04/2020.
//

#include <gtest/gtest.h>
#include <vector>
#include <path_matcher.h>
#include "../src/translate.h"

using namespace PathMatcher;
using namespace std;

namespace {
    TEST(Translate, ItTakesASetOfParserTokensAndReturnsARegex) {
        TokenCollection tokens = {
            "/test",
            ParseToken({.name="one", .prefix="/", .pattern=".*"}),
            ParseToken({.name="two", .prefix="/", .pattern=".*"}),
        };
        vector<ParseToken> results;

        auto regex = Translate::toRegex(tokens, results);

        EXPECT_STREQ(regex.str().c_str(), "^/test(?:/(.*))(?:/(.*))$");
        EXPECT_TRUE(boost::regex_match("/test/hello/world", regex));
    }

    TEST(Translate, ItPopulatesTheTokens) {
        TokenCollection tokens = {
            "/test",
            ParseToken({.name="one", .prefix="/", .pattern=".*"}),
            ParseToken({.name="two", .prefix="/", .pattern=".*"}),
            ParseToken({.name=1, .prefix="/", .pattern=".*"})
        };
        vector<ParseToken> results;

        Translate::toRegex(tokens, results);

        EXPECT_EQ(results.size(), 3);
        EXPECT_STREQ(get<string>(results[0].name).c_str(), "one");
        EXPECT_STREQ(get<string>(results[1].name).c_str(), "two");
        EXPECT_EQ(get<int>(results[2].name), 1);
    }
}