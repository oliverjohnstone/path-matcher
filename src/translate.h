//
// Created by Oliver Johnstone on 09/04/2020.
//

#ifndef PATH_MATCHER_TRANSLATE_H
#define PATH_MATCHER_TRANSLATE_H

#include <boost/regex.hpp>
#include <vector>
#include "parser.h"

namespace PathMatcher {
    class Translate {
    public:
        static boost::regex toRegex(const TokenCollection& tokensIn, std::vector<ParseToken>& tokensOut);
    };
}

#endif //PATH_MATCHER_TRANSLATE_H
