#ifndef PROTECTION_REGEXRULE_H
#define PROTECTION_REGEXRULE_H

#include "protection/Token.h"

#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace protection {
namespace injections {

class LIBPRROTECTION_EXPORT RegexRule final {
public:
  RegexRule() = delete;

  static RegexRule Token(const std::string &regexStr, TokenType type);

  static RegexRule TokenPushMode(const std::string &regexStr, TokenType type, const std::vector<RegexRule> &rules);

  static RegexRule TokenPopMode(const std::string &regexStr, TokenType type);

  static RegexRule NoToken(const std::string &regexStr);

  static RegexRule NoTokenPushMode(const std::string &regexStr, const std::vector<RegexRule> &rules);

  static RegexRule NoTokenPopMode(const std::string &regexStr);

  std::pair<size_t, bool> tryMatch(const std::string &text) const;

  TokenType getType() const;

  const std::vector<RegexRule> &getModeRules() const;

  bool isPushMode() const;

  bool isPopMode() const;

  bool isToken() const;

private:
  RegexRule(const std::string &regexStr, TokenType type);

  RegexRule(const std::string &regexStr, TokenType type, const std::vector<RegexRule> &rules);

  RegexRule(const std::string &regexStr, TokenType type, bool pop);

  explicit RegexRule(const std::string &regexStr);

  RegexRule(const std::string &regexStr, const std::vector<RegexRule> &rules);

  RegexRule(const std::string &regexStr, bool pop);

  const std::regex regex;
  TokenType tokenType{static_cast<size_t>(-1)};
  std::vector<RegexRule> modeRules;
  bool popMode{false};
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_REGEXRULE_H
