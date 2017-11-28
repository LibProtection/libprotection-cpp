#include "LanguageProvider.h"
#include "Range.h"

namespace protection {

Token LanguageProvider::createToken(L, TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) {
  return Token(type, lowerBound, upperBound, text, isTrivial(type, text));
}
}
