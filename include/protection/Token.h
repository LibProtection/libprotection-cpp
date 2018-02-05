#ifndef PROTECTION_TOKEN_H
#define PROTECTION_TOKEN_H

#include "protection/LanguageProvider.h"
#include "protection/Range.h"

#include <memory>
#include <string>

namespace protection {
namespace injections {

class LanguageProvider;

using TokenType = size_t;

struct Token {
  const LanguageProvider *languageProvider;
  TokenType tokenType;
  const std::string text;
  Range range;
  bool isTrivial;

  Token(const LanguageProvider *provider, TokenType type, size_t lowerBound, size_t upperBound, const std::string &t,
        bool isTrivial);

  std::string toString() const;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_TOKEN_H
