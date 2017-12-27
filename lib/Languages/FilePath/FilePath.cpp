#include "Languages/FilePath.h"

namespace protection {
namespace injections {

const std::vector<RegexRule> &FilePath::getMainModeRules() const {
  static const std::string disallowedSymbols = R"(<>:""/\\\|\?\*\x00-\x1f)";

  static const std::vector<RegexRule> mainModeRules = {
      RegexRule::Token(R"([\\/]+)", TOKEN_TYPE(FilePathTokenType::Separator)),
      RegexRule::Token(R"([a-zA-Z]+[\$:](?=[\\/]))", TOKEN_TYPE(FilePathTokenType::DeviceID)),
      RegexRule::Token(R"([^)" + disallowedSymbols + "]+", TOKEN_TYPE(FilePathTokenType::FSEntryName)),
      RegexRule::Token(R"(:+\$[^)" + disallowedSymbols + "]+", TOKEN_TYPE(FilePathTokenType::NTFSAttribute)),
      RegexRule::Token("[" + disallowedSymbols + "]", TOKEN_TYPE(FilePathTokenType::DisallowedSymbol))};

  return mainModeRules;
}

std::pair<std::string, bool> FilePath::trySanitize(const std::string &text, Token) const { return {{}, false}; };

Token FilePath::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(this, type, lowerBound, upperBound, text, isTrivial(type, text));
}

bool FilePath::isTrivial(TokenType type, const std::string &text) const {
  return (static_cast<FilePathTokenType>(type) == FilePathTokenType::FSEntryName) &&
         (text.find("..") == std::string::npos);
}

TokenType FilePath::getErrorTokenType() const { return TOKEN_TYPE(FilePathTokenType::Error); }

} // namespace injections
} // namespace protection
