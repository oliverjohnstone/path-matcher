//
// Created by Oliver Johnstone on 07/04/2020.
//

#include "parser.h"
#include <string>
#include <sstream>

using namespace std;

auto DEFAULT_PATTERN = "[^\\/#\?]+?";

auto excludePrefix = [](char ch) {
    return ch == '.' || ch == '/';
};

PathMatcher::Parser::Parser(std::vector<LexToken>& tokens) {
    parseLexTokens(tokens);
}

void PathMatcher::Parser::parseLexTokens(vector<LexToken> &lexTokens) {
    auto i = 0;
    auto key = 0;
    string path;

    auto tryConsume = [&lexTokens, &i](LexTokenType type) -> LexToken* {
        if (i < lexTokens.size() && lexTokens[i].type == type) {
            return &lexTokens[i++];
        }

        return nullptr;
    };

    auto consumeText = [tryConsume]() -> string {
        string result;
        LexToken *tok;

        while (true) {
            tok = tryConsume(LexTokenType::CHAR);
            if (!tok) {
                tok = tryConsume(LexTokenType::ESCAPED_CHAR);
            }

            if (!tok) {
                break;
            }

            result += tok->value;
        }

        return result;
    };

    auto mustConsume = [tryConsume, &lexTokens, i](LexTokenType type) -> LexToken* {
        auto valueTok = tryConsume(type);
        if (valueTok) {
            return valueTok;
        }

        auto nextTok = lexTokens[i];
        stringstream message;
        message << "Unexpected token type (" << nextTok.type << ") at position " << nextTok.index << ", expected type " << type;
        throw invalid_argument(message.str());
    };

    while (i < lexTokens.size()) {
        auto charTok = tryConsume(LexTokenType::CHAR);
        auto nameTok = tryConsume(LexTokenType::NAME);
        auto patternTok = tryConsume(LexTokenType::PATTERN);

        if (nameTok || patternTok) {
            string prefix = charTok ? charTok->value : "";

            if (!excludePrefix(prefix[0])) {
                path += prefix;
                prefix.clear();
            }

            if (path.length()) {
                tokens.emplace_back(path);
                path.clear();
            }

            auto modifier = tryConsume(LexTokenType::MODIFIER);

            ParseToken tok = {
                .prefix = prefix,
                .suffix = "",
                .pattern = patternTok ? patternTok->value : DEFAULT_PATTERN,
                .modifier = modifier ? modifier->value : ""
            };

            if (nameTok) {
                tok.name = nameTok->value;
            } else {
                tok.name = key++;
            }

            tokens.emplace_back(tok);
            continue;
        }

        auto valueTok = charTok ?: tryConsume(LexTokenType::ESCAPED_CHAR);
        if (valueTok) {
            path += valueTok->value;
            continue;
        }

        if (path.length()) {
            tokens.emplace_back(path);
            path.clear();
        }

        auto openTok = tryConsume(LexTokenType::OPEN);
        if (openTok) {
            auto prefix = consumeText();
            auto name = tryConsume(LexTokenType::NAME);
            auto pattern = tryConsume(LexTokenType::PATTERN);
            auto suffix = consumeText();

            mustConsume(LexTokenType::CLOSE);

            auto modTok = tryConsume(LexTokenType::MODIFIER);
            ParseToken tok = {
                .prefix = prefix,
                .suffix = suffix,
                .modifier = modTok ? modTok->value : ""
            };

            if (name) {
                if (pattern) {
                    tok.name = name->value;
                    tok.pattern = pattern->value;
                } else {
                    tok.name = name->value;
                    tok.pattern = DEFAULT_PATTERN;
                }
            } else if (pattern) {
                tok.name = key++;
                tok.pattern = pattern->value;
            }

            tokens.emplace_back(tok);
            continue;
        }

        mustConsume(LexTokenType::END);
    }
}
