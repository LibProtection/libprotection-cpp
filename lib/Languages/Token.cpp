#include <LanguageProvider.h>
#include "Token.h"

namespace protection {

Token::Token(LanguageProviderType providerType, TokenType type, size_t lowerBound, size_t upperBound,
             const std::string &t, bool trivial)
    : languageProviderType{providerType}, tokenType{type}, text{t}, range{lowerBound, upperBound}, isTrivial{trivial} {}

std::string Token::toString() const { return "\"" + text + "\""; }
} // namespace protection