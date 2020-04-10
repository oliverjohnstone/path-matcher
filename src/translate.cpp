//
// Created by Oliver Johnstone on 09/04/2020.
//

#include "translate.h"
#include <strstream>

using namespace std;

auto escape = [](const string& str) -> string {
    const boost::regex esc(R"([.^$|()\[\]{}*+?\\])");
    const string rep("\\\\$&");
    return boost::regex_replace(str, esc, rep, boost::match_default | boost::format_perl);
};

boost::regex PathMatcher::Translate::toRegex(const TokenCollection& tokensIn, vector<ParseToken>& tokensOut) {
    stringstream regex;
    regex << "^";

    for (auto &tokenRef : tokensIn) {
        if (auto str = get_if<string>(&tokenRef)) {
            regex << escape(*str);
            continue;
        }

        auto token = get_if<ParseToken>(&tokenRef);
        auto prefix = escape(token->prefix);
        auto suffix = escape(token->suffix);

        if (token->pattern.empty()) {
            regex << "(?:" << prefix << suffix << ")" << token->modifier;
            continue;
        }

        tokensOut.emplace_back(*token);

        if (prefix.empty() && suffix.empty()) {
            regex << "(" << token->pattern << ")" << token->modifier;
            continue;
        }

        if (token->modifier != "+" && token->modifier != "*") {
            regex << "(?:" << prefix << "(" << token->pattern << ")" << suffix << ")" << token->modifier;
            continue;
        }

        auto mod = token->modifier == "*" ? "?" : "";
        regex << "(?:" << prefix << "((?:" << token->pattern << ")(?:" << suffix;
        regex << prefix << "(?:" << token->pattern << "))*)" << suffix << ")" << mod;
    }

    regex << "$";

    return boost::regex(regex.str());
}
