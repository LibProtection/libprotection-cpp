#ifndef PROTECTION_LANGUAGEPROVIDER_H
#define PROTECTION_LANGUAGEPROVIDER_H

#include "protection/Range.h"
#include "protection/Token.h"

#include <utility>
#include <vector>

namespace protection {
namespace injections {

struct Token;

using TokenType = size_t;
#define TOKEN_TYPE(type) static_cast<TokenType>(type)

class LanguageProvider {
public:
  virtual ~LanguageProvider() = default;

  virtual std::vector<Token> tokenize(const std::string &text, size_t offset = 0) const = 0;

  virtual std::pair<std::string, bool> trySanitize(const std::string &text, const Token &context) const = 0;

  virtual std::string tokenTypeToString(TokenType type) const = 0;

protected:
  virtual Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const = 0;

  virtual bool isTrivial(TokenType type, const std::string &text) const = 0;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_LANGUAGEPROVIDER_H
