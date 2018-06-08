#include "protection/LanguageProvider.h"
#include "protection/Token.h"

namespace protection {
namespace injections {

Token::Token() : languageProvider{nullptr}, tokenType{static_cast<size_t>(-1)}, range{0, 0} {}

Token::Token(const LanguageProvider *provider, TokenType type, size_t lowerBound, size_t upperBound, std::string t,
             bool trivial)
    : languageProvider{provider}, tokenType{type}, text{std::move(t)}, range{lowerBound, upperBound}, isTrivial{
                                                                                                          trivial} {}

Token::Token(const Token &rhs) : range{rhs.range.lowerBound, rhs.range.upperBound} {
  languageProvider = rhs.languageProvider;
  tokenType = rhs.tokenType;
  text = rhs.text;
  isTrivial = rhs.isTrivial;
}

std::string Token::toString() const {
  return languageProvider->tokenTypeToString(tokenType) + R"(:")" + text + R"(":)" + range.toString();
}

} // namespace injections
} // namespace protection
