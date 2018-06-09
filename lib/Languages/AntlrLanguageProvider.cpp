#include "protection/AntlrLanguageProvider.h"

namespace protection {
namespace injections {

std::vector<Token> AntlrLanguageProvider::tokenize(const std::string &text, size_t offset) const {
  auto charStream = antlr4::ANTLRInputStream(text);
  auto lexer = createLexer(text, &charStream);
  auto antlrToken = lexer->nextToken();

  std::vector<Token> tokens;

  while (antlrToken->getType() != EOF) {
    tokens.push_back(createToken(convertAntlrTokenType(antlrToken->getType()), antlrToken->getStartIndex() + offset,
                                 antlrToken->getStopIndex() + offset + 1, antlrToken->getText()));

    antlrToken = lexer->nextToken();
  }

  return tokens;
}

} // namespace injections
} // namespace protection
