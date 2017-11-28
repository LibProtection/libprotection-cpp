#include <LanguageProvider.h>
#include "Token.h"
#include "Range.h"

namespace protection {

Token::Token(LanguageProviderType providerType, TokenType type, size_t lowerBound, size_t upperBound,
             const std::string &t, bool trivial)
    : languageProviderType{providerType}, tokenType{type}, text{t}, range{Range{lowerBound, upperBound}},
      isTrivial{trivial} {}

std::string Token::toString() const { return "\"" + text + "\""; }
}