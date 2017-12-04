#ifndef PROTECTION_TOKEN_H
#define PROTECTION_TOKEN_H

#include "LanguageProvider.h"
#include "Range.h"

#include <memory>
#include <string>

namespace protection {

enum class LanguageProviderType;
class LanguageProvider;

using TokenType = size_t;

struct Token {
  // LanguageProvider languageProvider;

  LanguageProviderType languageProviderType;
  TokenType tokenType;
  const std::string text;
  Range range;
  bool isTrivial;

  Token(LanguageProviderType providerType, TokenType type, size_t lowerBound, size_t upperBound, const std::string &t,
        bool isTrivial);
  std::string toString() const;
};
} // namespace protection

#endif // PROTECTION_TOKEN_H
