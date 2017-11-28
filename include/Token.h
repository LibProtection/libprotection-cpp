#ifndef PROTECTION_TOKEN_H
#define PROTECTION_TOKEN_H

#include "LanguageProvider.h"
#include "TokenType.h"
#include "Range.h"

#include <memory>
#include <string>

namespace protection {

enum class LanguageProviderType;
class LanguageProvider;

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
}

#endif // PROTECTION_TOKEN_H
