#include "Languages/FilePath.h"
#include "TokenType.h"

namespace protection {

const std::vector<RegexTokenDefinition> &FilePath::getTokenDefinitions() const {
  static const std::string disallowedSymbols = "<>:"
                                               "/\\\|\?\*\x00-\x1f";

  static const std::vector<RegexTokenDefinition> tokenDefinitions = {
      {"[\\/]+", TokenType::FilePathSeparator},
      {"[a-zA-Z]+[\$:](?=[\\/])", TokenType::FilePathDeviceID},
      {"[^" + disallowedSymbols + "]+", TokenType::FilePathFSEntryName},
      {":+\$[^" + disallowedSymbols + "]+", TokenType::FilePathNTFSAttribute},
      {disallowedSymbols, TokenType::FilePathDissallowedSymbol}};

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
}
