#ifndef PROTECTION_URL_H
#define PROTECTION_URL_H

#include "RegexLanguageProvider.h"
#include "RegexTokenDefinition.h"

namespace protection {
class Url final : public RegexLanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset = 0) override;
  std::pair<std::string, bool> trySanitize(const std::string &text, Token context) override;

private:
  bool isTrivial(TokenType type, const std::string &text) const override;

  const std::vector<RegexTokenDefinition> &getTokenDefinitions() const override;

  TokenType getErrorTokenType() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  std::vector<Token> splitToken(const std::string &text, size_t lowerBound, const std::string &splitChars,
                                TokenType type);

  std::pair<std::string, bool> tryUrlEncode(const std::string &text, TokenType tokenType);

  std::string UrlEncode(const std::string &text) { return text; }
};
}

#endif // PROTECTION_URL_H
