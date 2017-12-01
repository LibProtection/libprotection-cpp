#include "Languages/Sql.h"
#include "SQLLexer.h"

namespace protection {

std::unique_ptr<antlr4::Lexer> Sql::createLexer(const std::string &text) {
  return std::unique_ptr<sql::SQLLexer>(new sql::SQLLexer(new antlr4::ANTLRInputStream(text)));
}

TokenType Sql::convertAntlrTokenType(size_t antlrTokenType) { return TokenType::ErrorTokenType; }

} // namespace protection