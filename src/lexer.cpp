//
// Created by Oliver Johnstone on 08/04/2020.
//

#include <sstream>
#include "lexer.h"

using namespace std;

const char NAME_CHAR_RANGE[][2] = {{'0', '9'}, {'A', 'Z'}, {'a', 'z'}, {'_', '_'}};

auto isModifier = [](char ch) {
    return ch == '*' || ch == '+' || ch == '?';
};

auto isEscape = [](char ch) {
    return ch == '\\';
};

auto isOpen = [](char ch) {
    return ch == '{';
};

auto isClose = [](char ch) {
    return ch == '}';
};

auto isNameIndicator = [](char ch) {
    return ch == ':';
};

auto isValidNameChar = [](char ch) {
    for (auto &[start, finish] : NAME_CHAR_RANGE) {
        if (ch >= start && ch <= finish) {
            return true;
        }
    }

    return false;
};

auto isPatternIndicator = [](char ch) {
    return ch == '(';
};

auto isPatternClose = [](char ch) {
    return ch == ')';
};

PathMatcher::Lexer::Lexer(const std::string &path) : path(path) {
    parseTokens();
}

void PathMatcher::Lexer::parseTokens() {
    auto i = 0;
    for (; i < path.length(); i++) {
        auto ch = path[i];

        if (isModifier(ch)) {
            tokens.push_back({.type = LexTokenType::MODIFIER, .index = i, .value = string(1, ch)});
            continue;
        }

        if (isEscape(ch)) {
            tokens.push_back({.type = LexTokenType::ESCAPED_CHAR, .index = i, .value = string(1, ch)});
            continue;
        }

        if (isOpen(ch)) {
            tokens.push_back({.type = LexTokenType::OPEN, .index = i, .value = string(1, ch)});
            continue;
        }

        if (isClose(ch)) {
            tokens.push_back({.type = LexTokenType::CLOSE, .index = i, .value = string(1, ch)});
            continue;
        }

        if (isNameIndicator(ch)) {
            string name;
            auto nextPos = parseName(name, i);

            if (name.empty()) {
                stringstream message;
                message << "Missing argument name at position " << i << ".";
                throw invalid_argument(message.str());
            }

            tokens.push_back({.type = LexTokenType::NAME, .index = i, .value = name});
            i = nextPos;
            continue;
        }

        if (isPatternIndicator(ch)) {
            string pattern;
            auto nextPos = parsePattern(pattern, i);

            if (pattern.empty()) {
                stringstream message;
                message << "Missing pattern at position " << i << ".";
                throw invalid_argument(message.str());
            }

            tokens.push_back({.type = LexTokenType::PATTERN, .index = i, .value = pattern});
            i = nextPos;
            continue;
        }

        tokens.push_back({.type = LexTokenType::CHAR, .index = i, .value = string(1, ch)});
    }

    tokens.push_back({.type = LexTokenType::END, .index = i});
}

int PathMatcher::Lexer::parseName(std::string &nameOut, int indicatorPos) {
    for (auto i = indicatorPos + 1; i < path.length(); i++) {
        auto ch = path[i];
        if (isValidNameChar(ch)) {
            nameOut += ch;
            continue;
        }
        return i - 1;
    }

    return indicatorPos;
}

int PathMatcher::Lexer::parsePattern(std::string &patternOut, int patternPosition) {
    auto patternCount = 1;
    auto i = patternPosition + 1;

    if (path[i] == '?') {
        stringstream message;
        message << "Pattern can't start with a '?' at position " << i << ".";
        throw invalid_argument(message.str());
    }

    for (; i < path.length(); i++) {
        auto ch = path[i];
        if (isEscape(ch)) {
            patternOut += ch + path[i++];
            continue;
        }

        if (isPatternClose(ch)) {
            patternCount--;

            if (patternCount == 0) {
                break;
            }
        } else if (isPatternIndicator(ch)) {
            patternCount++;

            if (path[i] == '?') {
                stringstream message;
                message << "Capturing groups not allowed at position " << i << ".";
                throw invalid_argument(message.str());
            }
        }

        patternOut += ch;
    }

    if (patternCount) {
        stringstream message;
        message << "Unbalanced pattern at position " << i << ".";
        throw invalid_argument(message.str());
    }

    return i;
}
