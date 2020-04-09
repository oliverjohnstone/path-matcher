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
        Matcher matcher("/test/:idOne/:idTwo/three/(.*)/(.*)");

        auto result = matcher.matches("/test/123/456/three/captureGroup1", args);

        EXPECT_EQ(result, true);
    }
}