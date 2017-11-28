#ifndef PROTECTION_REGEXLANGUAGEPROVIDER_H
#define PROTECTION_REGEXLANGUAGEPROVIDER_H

#include "LanguageProvider.h"
#include "RegexTokenDefinition.h"

namespace protection {
class RegexLanguageProvider : public LanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) override;

private:
  virtual const std::vector<RegexTokenDefinition> &getTokenDefinitions() const = 0;

  virtual TokenType getErrorTokenType() const = 0;
};
}

#endif // PROTECTION_REGEXLANGUAGEPROVIDER_H
