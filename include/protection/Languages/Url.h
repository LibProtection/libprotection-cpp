#ifndef PROTECTION_URL_H
#define PROTECTION_URL_H

#include "protection/RegexLanguageProvider.h"
#include "protection/Single.h"

namespace protection {
namespace injections {

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
  std::pair<std::string, bool> trySanitize(const std::string &text, const Token &context) const override;

  std::string tokenTypeToString(TokenType type) const override;

private:
  Url() = default;

  bool isTrivial(TokenType type, const std::string &text) const override;

  const std::vector<RegexRule> &getMainModeRules() const override;

  TokenType getErrorTokenType() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  std::pair<std::string, bool> tryUrlEncode(const std::string &text, TokenType tokenType) const;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_URL_H
