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

    TEST(PathMatcherTest, SupportsNoRoute) {
        ArgResults args;
        Matcher matcher("");

        EXPECT_TRUE(matcher.matches("", args));
        EXPECT_TRUE(args.empty());
    }

    TEST(PathMatcherTest, SupportsRegexCaptureGroups) {
        ArgResults args;
        Matcher matcher("/(.*)/(.*)");

        EXPECT_TRUE(matcher.matches("/test/arg1", args));
        EXPECT_EQ(args.size(), 2);
        EXPECT_STREQ(args["captureGroup_0"].c_str(), "test");
        EXPECT_STREQ(args["captureGroup_1"].c_str(), "arg1");
    }

    TEST(PathMatcherTest, ReturnsExpectedResultWithNoMatch) {
        ArgResults args;
        Matcher matcher("/test");

        EXPECT_FALSE(matcher.matches("/test123", args));
        EXPECT_TRUE(args.empty());
    }

    TEST(PathMatcherTest, SupportsMultipleCallsToMatches) {
        Matcher matcher("/test/:arg1/route/:arg2");

        for (int i = 0; i < 1000; i++) {
            ArgResults args;
            EXPECT_TRUE(matcher.matches("/test/123/route/hello", args));
            EXPECT_STREQ(args["arg1"].c_str(), "123");
            EXPECT_STREQ(args["arg2"].c_str(), "hello");

            args.clear();
            EXPECT_FALSE(matcher.matches("/test/hi", args));
            EXPECT_TRUE(args.empty());
        }
    }
}