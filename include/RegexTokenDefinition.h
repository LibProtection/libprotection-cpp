#ifndef PROTECTION_REGEXTOKENDEFINITION_H
#define PROTECTION_REGEXTOKENDEFINITION_H

#include "TokenType.h"

#include <regex>
#include <string>
#include <utility>

namespace protection {

class RegexTokenDefinition {
public:
  RegexTokenDefinition(const std::string &regexStr, TokenType type);

  std::pair<size_t, bool> tryMatch(const std::string &text) const;

private:
  const std::regex regex;
  TokenType tokenType;
};
}
#endif // PROTECTION_REGEXTOKENDEFINITION_H
