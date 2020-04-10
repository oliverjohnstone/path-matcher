//
// Created by Oliver Johnstone on 07/04/2020.
//

#include <gtest/gtest.h>
#include <path_matcher.h>

using namespace PathMatcher;
using namespace std;

namespace {
    TEST(PathMatcherTest, SupportsBasicRoutes) {
        ArgResults args;
        Matcher matcher("/test/:testArg/:testArg2");

        EXPECT_TRUE(matcher.matches("/test/test2/test3", args));
        EXPECT_EQ(args.size(), 2);
        EXPECT_STREQ(args["testArg"].c_str(), "test2");
        EXPECT_STREQ(args["testArg2"].c_str(), "test3");
    }
}