#ifndef PROTECTION_FILEPATH_H
#define PROTECTION_FILEPATH_H

#include <string>
#include <vector>

#include "Token.h"
#include "RegexLanguageProvider.h"
#include "Single.h"

namespace protection {
namespace injections {

enum class FilePathTokenType { Error, DisallowedSymbol, DeviceID, FSEntryName, NTFSAttribute, Separator };

class FilePath final : public RegexLanguageProvider {
  friend Single<FilePath>;

public:
  std::pair<std::string, bool> trySanitize(const std::string &text, Token) const override;

  std::string tokenTypeToString(TokenType type) const override;

private:
  FilePath() = default;

  bool isTrivial(TokenType type, const std::string &text) const override;

  const std::vector<RegexRule> &getMainModeRules() const override;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  TokenType getErrorTokenType() const override;
};

} // namespace injections
} // namespace protection
#endif // PROTECTION_FILEPATH_H
