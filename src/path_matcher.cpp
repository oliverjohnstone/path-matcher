//
// Created by Oliver Johnstone on 07/04/2020.
//

#include <path_matcher.h>
#include "lexer.h"
#include "parser.h"
#include "translate.h"

#include <iostream>

using namespace std;

auto zip = [](vector<PathMatcher::ParseToken>& tokens, vector<string>& matches, PathMatcher::ArgResults& out) {
    for (int i = 0; i < tokens.size() && i < matches.size(); i++) {
        auto token = tokens[i];
        string key;

        if (auto str = get_if<string>(&token.name)) {
            key = *str;
        } else {
            stringstream ss;
            ss << "captureGroup_" << get<int>(token.name);
            key = ss.str();
        }

        out[key] = matches[i];
    }
};

class PathMatcher::Matcher::Implementation {
public:
    vector<ParseToken> tokens;
    boost::regex regex;

    explicit Implementation(const string& pathToMatch) {
        Lexer lexer(pathToMatch);
        Parser parser(lexer.getTokens());
        regex = Translate::toRegex(parser.getTokens(), tokens);
    }
};

PathMatcher::Matcher::Matcher(const string& pathToMatch) : implementation(new Implementation(pathToMatch)) {
}

bool PathMatcher::Matcher::matches(const string& text, ArgResults& argsOut) {
    boost::smatch what;
    vector<string> matches;

    if (!boost::regex_match(text, what, implementation->regex, boost::match_extra)) {
        return false;
    }

    for (int i = 1; i < what.size(); ++i) {
        matches.emplace_back(what[i]);
    }

    if (matches.size() != implementation->tokens.size()) {
        stringstream msg;
        msg << "Invalid number of matches, expected " << implementation->tokens.size() << " but got " << matches.size() << ".";
        throw invalid_argument(msg.str());
    }

    zip(implementation->tokens, matches, argsOut);

    return true;
}

PathMatcher::Matcher::~Matcher() {
    delete implementation;
}
