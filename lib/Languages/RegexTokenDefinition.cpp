
#include <regex>
#include "RegexTokenDefinition.h"

namespace protection {

RegexTokenDefinition::RegexTokenDefinition(const std::string &regexStr, TokenType type)
    : regex{std::regex{"^" + regexStr}}, tokenType{type} {}

std::pair<size_t, bool> RegexTokenDefinition::tryMatch(const std::string &text) const {
  std::smatch sm;
  auto result = std::regex_search(text, sm, regex);
  return {(result) ? sm[0].length() : 0, result};
};
}
