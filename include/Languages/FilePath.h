#ifndef PROTECTION_FILEPATH_H
#define PROTECTION_FILEPATH_H

#include <string>
#include <vector>

#include "Token.h"
#include "RegexTokenDefinition.h"
#include "RegexLanguageProvider.h"

namespace protection {

class FilePath final : public RegexLanguageProvider {
public:
  FilePath() = default;

  std::pair<std::string, bool> trySanitize(const std::string &text, Token context) override;

protected:
  bool isTrivial(TokenType type, const std::string &text) const override;

private:
  const std::vector<RegexTokenDefinition> &getTokenDefinitions() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  TokenType getErrorTokenType() const override;
};
}
#endif // PROTECTION_FILEPATH_H
