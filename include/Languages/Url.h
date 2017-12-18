#ifndef PROTECTION_URL_H
#define PROTECTION_URL_H

#include "RegexLanguageProvider.h"
#include "Single.h"

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
  friend Single<Url>;

public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) const override;
  std::pair<std::string, bool> trySanitize(const std::string &text, Token context) const override;

private:
  Url() = default;

  bool isTrivial(TokenType type, const std::string &text) const override;

  const std::vector<RegexRule> &getMainModeRules() const override;

  TokenType getErrorTokenType() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  std::vector<Token> splitToken(const std::string &text, size_t lowerBound, const std::string &splitChars,
                                TokenType type) const;

  std::pair<std::string, bool> tryUrlEncode(const std::string &text, TokenType tokenType) const;
};
} // namespace protection

#endif // PROTECTION_URL_H
