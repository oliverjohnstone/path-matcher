//
// Created by Oliver Johnstone on 07/04/2020.
//

#ifndef PATH_MATCHER_PATH_MATCHER_H
#define PATH_MATCHER_PATH_MATCHER_H

#include <string>

namespace PathMatcher {
    class Matcher {
    private:
        class Implementation;
        Implementation *implementation;

    public:
        explicit Matcher(const std::string& pathToMatch);
        virtual ~Matcher();
        bool matches(const std::string& path, std::vector<std::string> &argsOut);
    };
}

#endif //PATH_MATCHER_PATH_MATCHER_H
