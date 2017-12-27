#include "Languages/JavaScript.h"
#include "JavaScriptCppLexer.h"
#include "Support/WebUtils.h"

namespace protection {
namespace injections {

Token JavaScript::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(this, type, lowerBound, upperBound, text, isTrivial(type, text));
}

TokenType JavaScript::convertAntlrTokenType(size_t antlrTokenType) const { return antlrTokenType; }

std::unique_ptr<antlr4::Lexer> JavaScript::createLexer(const std::string &text) const {
  return std::unique_ptr<javascript::JavaScriptCppLexer>(
      new javascript::JavaScriptCppLexer(new antlr4::ANTLRInputStream(text)));
}

bool JavaScript::isTrivial(TokenType type, const std::string &) const {
  switch ((JavaScriptTokenType)type) {
  case JavaScriptTokenType::LineTerminator:

  case JavaScriptTokenType::MultiLineComment:
  case JavaScriptTokenType::SingleLineComment:

  case JavaScriptTokenType::RegularExpressionLiteral:
  case JavaScriptTokenType::NullLiteral:
  case JavaScriptTokenType::BooleanLiteral:
  case JavaScriptTokenType::DecimalLiteral:
  case JavaScriptTokenType::HexIntegerLiteral:
  case JavaScriptTokenType::OctalIntegerLiteral:
  case JavaScriptTokenType::StringLiteral:
    return true;
  default:
    break;
  }

  return false;
}

std::pair<std::string, bool> JavaScript::trySanitize(const std::string &text, Token context) const {
  if (dynamic_cast<decltype(this)>(context.languageProvider)) {
    auto encodeResult = tryJavaScriptEncode(text, context.tokenType);
    if (encodeResult.second) {
      return encodeResult;
    }
  } else {
    throw std::runtime_error{"Unsupported JavaScript island: " + context.toString()};
  }

  return {{}, false};
}

std::pair<std::string, bool> JavaScript::tryJavaScriptEncode(const std::string &text, TokenType tokenType) const {
  switch (static_cast<JavaScriptTokenType>(tokenType)) {
  case JavaScriptTokenType::RegularExpressionLiteral: {

    auto replaceAll = [](std::string &str, const std::string &search, const std::string &replace) {
      std::string::size_type pos = 0;
      while ((pos = str.find(search, pos)) != std::string::npos) {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
      }
    };
    auto encodeResult = utility::JavaScriptStringEncode(text);
    replaceAll(encodeResult, "/", R"(\/)");
    return {encodeResult, true};
  }

  case JavaScriptTokenType::StringLiteral:
    return {utility::JavaScriptStringEncode(text), true};
  default:
    break;
  }

  return {{}, false};
}

} // namespace injections
} // namespace protection