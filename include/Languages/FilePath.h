#ifndef PROTECTION_FILEPATH_H
#define PROTECTION_FILEPATH_H

#include <string>
#include <vector>

#include "Token.h"
#include "RegexLanguageProvider.h"

namespace protection {

enum class FilePathTokenType { Error, DisallowedSymbol, DeviceID, FSEntryName, NTFSAttribute, Separator };

class FilePath final : public RegexLanguageProvider {
public:
  FilePath() = default;

  std::pair<std::string, bool> trySanitize(const std::string &text, Token) override;

protected:
  bool isTrivial(TokenType type, const std::string &text) const override;

private:
  const std::vector<RegexRule> &getMainModeRules() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  TokenType getErrorTokenType() const override;
};
} // namespace protection
#endif // PROTECTION_FILEPATH_H
