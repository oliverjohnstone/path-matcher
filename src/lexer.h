//
// Created by Oliver Johnstone on 08/04/2020.
//

#ifndef PATH_MATCHER_LEXER_H
#define PATH_MATCHER_LEXER_H

#include <string>
#include <vector>

namespace PathMatcher {
    enum LexTokenType {OPEN, CLOSE, PATTERN, NAME, CHAR, ESCAPED_CHAR, MODIFIER, END};

    typedef struct {
        LexTokenType type;
        int index;
        std::string value;
    } LexToken;

    class Lexer {
    private:
        const std::string& path;
        std::vector<LexToken> tokens;

        void parseTokens();
        int parseName(std::string &nameOut, int indicatorPos);
        int parsePattern(std::string &patternOut, int patternPosition);

    public:
        Lexer(const std::string &path);
        std::vector<LexToken>& getTokens() { return tokens; }
        auto size() { return tokens.size(); }
    };
}

#endif //PATH_MATCHER_LEXER_H
