#include "Languages/FilePath.h"

namespace protection {

const std::vector<RegexTokenDefinition> &FilePath::getTokenDefinitions() const {
  static const std::string disallowedSymbols = R"(<>:""/\\\|\?\*\x00-\x1f)";

  static const std::vector<RegexTokenDefinition> tokenDefinitions = {
      {R"([\\/]+)", FilePathTokenType ::Separator},
      {R"([a-zA-Z]+[\$:](?=[\\/]))", FilePathTokenType::DeviceID},
      {R"([^)" + disallowedSymbols + "]+", FilePathTokenType::FSEntryName},
      {R"(:+\$[^)" + disallowedSymbols + "]+", FilePathTokenType::NTFSAttribute},
      {"[" + disallowedSymbols + "]", FilePathTokenType::DisallowedSymbol}};

  return tokenDefinitions;
}

std::pair<std::string, bool> FilePath::trySanitize(const std::string &text, Token context) { return {{}, false}; };

Token FilePath::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(LanguageProviderType::FilePath, type, lowerBound, upperBound, text, isTrivial(type, text));
}

bool FilePath::isTrivial(TokenType type, const std::string &text) const {
  return (type == FilePathTokenType::FSEntryName) && (text.find("..") == std::string::npos);
}

TokenType FilePath::getErrorTokenType() const { return FilePathTokenType::Error; }
} // namespace protection
