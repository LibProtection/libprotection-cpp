#ifndef PROTECTION_SQL_H
#define PROTECTION_SQL_H

#include "AntlrLanguageProvider.h"

namespace protection {

class Sql final : public AntlrLanguageProvider {
public:
  Sql() = default;

private:
  TokenType convertAntlrTokenType(size_t antlrTokenType) override;

  std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text) override;
};

} // namespace protection

#endif // PROTECTION_SQL_H
