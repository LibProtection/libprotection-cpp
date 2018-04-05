#ifndef PROTECTION_ANTLRLANGUAGEPROVIDER_H
#define PROTECTION_ANTLRLANGUAGEPROVIDER_H

#include "antlr4-runtime.h"
#include "protection/LanguageProvider.h"

#include <memory>

namespace protection {
namespace injections {

class LIBPRROTECTION_EXPORT AntlrLanguageProvider : public LanguageProvider {
public:
  std::vector<Token> tokenize(const std::string &text, size_t offset = 0) const override;

protected:
  static const size_t EOF = static_cast<size_t>(-1);

  virtual TokenType convertAntlrTokenType(size_t antlrTokenType) const = 0;

  virtual std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text, antlr4::CharStream *charStream) const = 0;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_ANTLRLANGUAGEPROVIDER_H
