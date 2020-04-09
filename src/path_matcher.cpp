//
// Created by Oliver Johnstone on 07/04/2020.
//

#include <path_matcher.h>
#include "lexer.h"
#include "parser.h"


#include <iostream>

using namespace std;

class PathMatcher::Matcher::Implementation {
private:
public:
    explicit Implementation(const string& pathToMatch) {
        Lexer lexer(pathToMatch);
        Parser parser(lexer.getTokens());

        for (auto &token : parser) {
            if (auto tok = std::get_if<ParseToken>(&token)) {
                if (auto nameInt = std::get_if<int>(&tok->name)) {
                    std::cout << *nameInt << " " << tok->pattern << std::endl;
                } else {
                    auto nameStr = std::get<string>(tok->name);
                    std::cout << tok->prefix << " " << nameStr << tok->suffix << std::endl;
                }
            } else {
                auto tokStr = get<string>(token);
                std::cout << "STRING " << tokStr << std::endl;
            }
        }
    }
};

PathMatcher::Matcher::Matcher(const string& pathToMatch) : implementation(new Implementation(pathToMatch)) {
}

bool PathMatcher::Matcher::matches(const string& path, vector<string>& argsOut) {
    return false;
}

PathMatcher::Matcher::~Matcher() {
    delete implementation;
}
