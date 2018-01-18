#ifndef PROTECTION_HTML_H
#define PROTECTION_HTML_H

#include "AntlrLanguageProvider.h"
#include "Token.h"
#include "Single.h"

#include <set>

namespace protection {
namespace injections {

enum class HtmlTokenType {
  HtmlComment = 1,
  HtmlConditionalComment = 2,
  XmlDeclaration = 3,
  Cdata = 4,
  Dtd = 5,
  SpecialTag = 6,
  TagOpen = 7,
  HtmlText = 8,
  ErrorText = 9,
  TagClose = 10,
  TagSlashClose = 11,
  TagSlash = 12,
  TagEquals = 13,
  TagWhiteSpace = 14,
  AttributeName = 15,
  ErrorTag = 16,
  AttributeWhiteSpace = 17,
  AttributeSlash = 18,
  AttributeValue = 19,
  ErrorAttribute = 20
};

enum class HtmlTokenizerState {
  Insignificant,
  EventName,
  EventEqualSign,
  EventValue,
  ResourceName,
  ResourceEqualSign,
  ResourceValue
};

class Html final : public AntlrLanguageProvider {
  friend Single<Html>;

public:
  std::vector<Token> tokenize(const std::string &text, size_t offset = 0) const override;

  std::pair<std::string, bool> trySanitize(const std::string &text, Token context) const override;

  std::string tokenTypeToString(TokenType type) const override;

private:
  Html() = default;

  Token createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const override;

  TokenType convertAntlrTokenType(size_t antlrTokenType) const override;

  std::unique_ptr<antlr4::Lexer> createLexer(const std::string &text) const override;

  bool isTrivial(TokenType type, const std::string &text) const override;

  struct IslandDto;

  std::unique_ptr<IslandDto> isContextChanged(const Token &htmlToken, HtmlTokenizerState context,
                                              bool insideScriptTag) const;

  std::string trimQuotes(const Token &token, size_t &offset) const;

  std::string htmlEncode(const std::string &text, HtmlTokenType type) const;

  static const std::set<std::string> htmlUrlAttributes;
};

} // namespace injections
} // namespace protection
#endif // PROTECTION_HTML_H
