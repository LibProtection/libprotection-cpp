#ifndef PROTECTION_ANTLRLANGUAGEPROVIDER_H
#define PROTECTION_ANTLRLANGUAGEPROVIDER_H

#include "LanguageProvider.h"
#include "antlr4-runtime.h"

#include <memory>

namespace protection {
namespace injections {

class AntlrLanguageProvider : public LanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) const override;

protected:
  static const size_t EOF = static_cast<size_t>(-1);

  virtual TokenType convertAntlrTokenType(size_t antlrTokenType) const = 0;

  virtual std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text) const = 0;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_ANTLRLANGUAGEPROVIDER_H
