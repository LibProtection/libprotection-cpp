#include "AntlrLanguageProvider.h"

namespace protection {

std::vector<Token> AntlrLanguageProvider::tokenize(const std::string &text, size_t offset) {
  auto lexer = createLexer(text);
  auto antlrToken = lexer->nextToken();

  std::vector<Token> tokens;

  while (antlrToken->getType() != EOF) {
    tokens.push_back(createToken(convertAntlrTokenType(antlrToken->getType()), antlrToken->getStartIndex() + offset,
                                 antlrToken->getStopIndex() + offset, antlrToken->getText()));

    antlrToken = lexer->nextToken();
  }

  return tokens;
}
} // namespace protection