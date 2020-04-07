//
// Created by Oliver Johnstone on 07/04/2020.
//

#ifndef PATH_MATCHER_PATH_MATCHER_H
#define PATH_MATCHER_PATH_MATCHER_H

#include <string>

namespace PathMatcher {
    class Matcher {
    public:
        explicit Matcher(std::string& pathToMatch);
        bool matches(std::string path, std::vector<std::string> &argsOut);
    };
}

#endif //PATH_MATCHER_PATH_MATCHER_H
