#include <LanguageProvider.h>
#include "Token.h"

namespace protection {
namespace injections {

Token::Token(const LanguageProvider *provider, TokenType type, size_t lowerBound, size_t upperBound,
             const std::string &t, bool trivial)
    : languageProvider{provider}, tokenType{type}, text{t}, range{lowerBound, upperBound}, isTrivial{trivial} {}

std::string Token::toString() const { return "\"" + text + "\":" + range.toString(); }

} // namespace injections
} // namespace protection