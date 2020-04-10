//
// Created by Oliver Johnstone on 07/04/2020.
//

#ifndef PATH_MATCHER_PARSER_H
#define PATH_MATCHER_PARSER_H

#include "lexer.h"
#include <vector>
#include <variant>

namespace PathMatcher {
    typedef struct {
        std::variant<std::string, int> name;
        std::string prefix;
        std::string suffix;
        std::string pattern;
        std::string modifier;
    } ParseToken;

    typedef std::vector<std::variant<ParseToken, std::string>> TokenCollection;

    class Parser {
    private:
        TokenCollection tokens;

    public:
        explicit Parser(std::vector<LexToken>& tokens);
        void parseLexTokens(std::vector<LexToken> &lexTokens);
        [[nodiscard]] const TokenCollection &getTokens() const;
    };
}

#endif //PATH_MATCHER_PARSER_H
