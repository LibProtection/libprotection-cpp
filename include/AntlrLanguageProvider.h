#ifndef PROTECTION_ANTLRLANGUAGEPROVIDER_H
#define PROTECTION_ANTLRLANGUAGEPROVIDER_H

#include "LanguageProvider.h"
#include "antlr4-runtime.h"

#include <memory>

namespace protection {

class AntlrLanguageProvider : public LanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset) override;

protected:
  static const size_t EOF = static_cast<size_t>(-1);

  virtual TokenType convertAntlrTokenType(size_t antlrTokenType) = 0;

  virtual std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text) = 0;
};

} // namespace protection

#endif // PROTECTION_ANTLRLANGUAGEPROVIDER_H
