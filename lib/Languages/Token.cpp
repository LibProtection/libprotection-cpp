#include "protection/LanguageProvider.h"
#include "protection/Token.h"

namespace protection {
namespace injections {

Token::Token(const LanguageProvider *provider, TokenType type, size_t lowerBound, size_t upperBound,
             const std::string &t, bool trivial)
    : languageProvider{provider}, tokenType{type}, text{t}, range{lowerBound, upperBound}, isTrivial{trivial} {}

Token::Token(const Token &rhs) : range{rhs.range.lowerBound, rhs.range.upperBound} {
  languageProvider = rhs.languageProvider;
  tokenType = rhs.tokenType;
  text = rhs.text;
  isTrivial = rhs.isTrivial;
}

std::string Token::toString() const {
  return languageProvider->tokenTypeToString(tokenType) + ":\"" + text + "\":" + range.toString();
}

} // namespace injections
} // namespace protection