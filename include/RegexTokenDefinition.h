#ifndef PROTECTION_REGEXTOKENDEFINITION_H
#define PROTECTION_REGEXTOKENDEFINITION_H

#include "Token.h"

#include <regex>
#include <string>
#include <utility>

namespace protection {

class RegexTokenDefinition {
public:
  RegexTokenDefinition(const std::string &regexStr, TokenType type);

  std::pair<size_t, bool> tryMatch(const std::string &text) const;

  TokenType getType() const;

private:
  const std::regex regex;
  TokenType tokenType;
};
} // namespace protection
#endif // PROTECTION_REGEXTOKENDEFINITION_H
