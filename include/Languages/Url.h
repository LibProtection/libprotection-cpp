#ifndef PROTECTION_URL_H
#define PROTECTION_URL_H

#include "RegexLanguageProvider.h"

namespace protection {

enum class UrlTokenType {
  Error,
  Separator,
  Scheme,
  AuthorityEntry,
  PathEntry,
  QueryEntry,
  Fragment,

  // Non-terminals
  SchemeCtx,
  AuthorityCtx,
  PathCtx,
  QueryCtx,
  FragmentCtx
};

class Url final : public RegexLanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) override;
  std::pair<std::string, bool> trySanitize(const std::string &text, Token context) override;

private:
  bool isTrivial(TokenType type, const std::string &text) const override;

  const std::vector<RegexRule> &getMainModeRules() const override;

  TokenType getErrorTokenType() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  std::vector<Token> splitToken(const std::string &text, size_t lowerBound, const std::string &splitChars,
                                TokenType type);

  std::pair<std::string, bool> tryUrlEncode(const std::string &text, TokenType tokenType);

  std::string UrlEncode(const std::string &text) { return text; }
};
} // namespace protection

#endif // PROTECTION_URL_H
