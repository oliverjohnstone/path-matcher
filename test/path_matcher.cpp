//
// Created by Oliver Johnstone on 07/04/2020.
//

#include <gtest/gtest.h>
#include <path_matcher.h>

using namespace PathMatcher;
using namespace std;

namespace {
    TEST(PathMatcherTest, SupportsBasicRoutes) {
        vector<string> args;
        string path = "/test/123";
        Matcher matcher(path);

        auto result = matcher.matches("/test/123", args);

        EXPECT_EQ(result, true);
    }
}