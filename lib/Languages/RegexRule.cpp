#include "RegexRule.h"

namespace protection {

/// Token
RegexRule::RegexRule(const std::string &regexStr, TokenType type)
    : regex{std::regex{"^" + regexStr}}, tokenType{type} {}

/// TokenPushMode
RegexRule::RegexRule(const std::string &regexStr, TokenType type, const std::vector<RegexRule> &rules)
    : regex{std::regex{"^" + regexStr}}, tokenType{type}, modeRules{rules} {}

/// TokenPopMode
RegexRule::RegexRule(const std::string &regexStr, TokenType type, bool pop)
    : regex{std::regex{"^" + regexStr}}, tokenType{type}, popMode{pop} {}

/// NoToken
RegexRule::RegexRule(const std::string &regexStr) : regex{std::regex{"^" + regexStr}} {}

/// NoTokenPushMode
RegexRule::RegexRule(const std::string &regexStr, const std::vector<RegexRule> &rules)
    : regex{std::regex{"^" + regexStr}}, modeRules{rules} {}

/// NoTokenPopMode
RegexRule::RegexRule(const std::string &regexStr, bool pop) : regex{std::regex{"^" + regexStr}}, popMode{pop} {}

std::pair<size_t, bool> RegexRule::tryMatch(const std::string &text) const {
  std::smatch sm;
  auto result = std::regex_search(text, sm, regex);
  return {(result) ? static_cast<size_t>(sm[0].length()) : 0, result};
}

TokenType RegexRule::getType() const { return tokenType; }

const std::vector<RegexRule> &RegexRule::getModeRules() const { return modeRules; }

bool RegexRule::isPushMode() const { return !modeRules.empty(); }

bool RegexRule::isPopMode() const { return popMode; }

bool RegexRule::isToken() const { return tokenType != static_cast<size_t>(-1); }

RegexRule RegexRule::Token(const std::string &regexStr, TokenType type) { return RegexRule(regexStr, type); }

RegexRule RegexRule::TokenPushMode(const std::string &regexStr, TokenType type, const std::vector<RegexRule> &rules) {
  return RegexRule(regexStr, type, rules);
}

RegexRule RegexRule::TokenPopMode(const std::string &regexStr, TokenType type) {
  return RegexRule(regexStr, type, true);
}

RegexRule RegexRule::NoToken(const std::string &regexStr) { return RegexRule(regexStr); }

RegexRule RegexRule::NoTokenPushMode(const std::string &regexStr, const std::vector<RegexRule> &rules) {
  return RegexRule(regexStr, rules);
}

RegexRule RegexRule::NoTokenPopMode(const std::string &regexStr) { return RegexRule(regexStr, true); }

} // namespace protection