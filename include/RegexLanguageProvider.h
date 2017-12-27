#ifndef PROTECTION_REGEXLANGUAGEPROVIDER_H
#define PROTECTION_REGEXLANGUAGEPROVIDER_H

#include "LanguageProvider.h"
#include "RegexRule.h"

namespace protection {
namespace injections {

class RegexLanguageProvider : public LanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) const override;

private:
  virtual const std::vector<RegexRule> &getMainModeRules() const = 0;

  virtual TokenType getErrorTokenType() const = 0;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_REGEXLANGUAGEPROVIDER_H
