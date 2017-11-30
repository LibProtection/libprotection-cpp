#include "Languages/FilePath.h"
#include "TokenType.h"

namespace protection {

const std::vector<RegexTokenDefinition> &FilePath::getTokenDefinitions() const {
  static const std::string disallowedSymbols = R"(<>:""/\\\|\?\*\x00-\x1f)";

  static const std::vector<RegexTokenDefinition> tokenDefinitions = {
      {R"([\\/]+)", TokenType::FilePathSeparator},
      {R"([a-zA-Z]+[\$:](?=[\\/]))", TokenType::FilePathDeviceID},
      {R"([^)" + disallowedSymbols + "]+", TokenType::FilePathFSEntryName},
      {R"(:+\$[^)" + disallowedSymbols + "]+", TokenType::FilePathNTFSAttribute},
      {"[" + disallowedSymbols + "]", TokenType::FilePathDissallowedSymbol}};

  return tokenDefinitions;
}

std::pair<std::string, bool> FilePath::trySanitize(const std::string &text, Token context) { return {{}, false}; };

Token FilePath::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(LanguageProviderType::FilePath, type, lowerBound, upperBound, text, isTrivial(type, text));
}

bool FilePath::isTrivial(TokenType type, const std::string &text) const {
  return (type == TokenType::FilePathFSEntryName) && (text.find("..") == std::string::npos);
}

TokenType FilePath::getErrorTokenType() const { return TokenType::FilePathError; }
} // namespace protection
