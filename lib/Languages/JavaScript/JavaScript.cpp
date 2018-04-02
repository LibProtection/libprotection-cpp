#include "protection/Languages/JavaScript.h"
#include "JavaScriptCppLexer.h"
#include "support/WebUtils.h"

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

bool JavaScript::isTrivial(TokenType type, const std::string & /*unused*/) const {
  switch (static_cast<JavaScriptTokenType>(type)) {
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

std::pair<std::string, bool> JavaScript::trySanitize(const std::string &text, const Token &context) const {
  if (dynamic_cast<decltype(this)>(context.languageProvider) != nullptr) {
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

std::string JavaScript::tokenTypeToString(TokenType type) const {
  static const std::map<JavaScriptTokenType, std::string> tokenTypeToStringMap = {
      {JavaScriptTokenType::RegularExpressionLiteral, "RegularExpressionLiteral"},
      {JavaScriptTokenType::LineTerminator, "LineTerminator"},
      {JavaScriptTokenType::OpenBracket, "OpenBracket"},
      {JavaScriptTokenType::CloseBracket, "CloseBracket"},
      {JavaScriptTokenType::OpenParen, "OpenParen"},
      {JavaScriptTokenType::CloseParen, "CloseParen"},
      {JavaScriptTokenType::OpenBrace, "OpenBrace"},
      {JavaScriptTokenType::CloseBrace, "CloseBrace"},
      {JavaScriptTokenType::SemiColon, "SemiColon"},
      {JavaScriptTokenType::Comma, "Comma"},
      {JavaScriptTokenType::Assign, "Assign"},
      {JavaScriptTokenType::QuestionMark, "QuestionMark"},
      {JavaScriptTokenType::Colon, "Colon"},
      {JavaScriptTokenType::Dot, "Dot"},
      {JavaScriptTokenType::PlusPlus, "PlusPlus"},
      {JavaScriptTokenType::MinusMinus, "MinusMinus"},
      {JavaScriptTokenType::Plus, "Plus"},
      {JavaScriptTokenType::Minus, "Minus"},
      {JavaScriptTokenType::BitNot, "BitNot"},
      {JavaScriptTokenType::Not, "Not"},
      {JavaScriptTokenType::Multiply, "Multiply"},
      {JavaScriptTokenType::Divide, "Divide"},
      {JavaScriptTokenType::Modulus, "Modulus"},
      {JavaScriptTokenType::RightShiftArithmetic, "RightShiftArithmetic"},
      {JavaScriptTokenType::LeftShiftArithmetic, "LeftShiftArithmetic"},
      {JavaScriptTokenType::RightShiftLogical, "RightShiftLogical"},
      {JavaScriptTokenType::LessThan, "LessThan"},
      {JavaScriptTokenType::MoreThan, "MoreThan"},
      {JavaScriptTokenType::LessThanEquals, "LessThanEquals"},
      {JavaScriptTokenType::GreaterThanEquals, "GreaterThanEquals"},
      {JavaScriptTokenType::Equals, "Equals"},
      {JavaScriptTokenType::NotEquals, "NotEquals"},
      {JavaScriptTokenType::IdentityEquals, "IdentityEquals"},
      {JavaScriptTokenType::IdentityNotEquals, "IdentityNotEquals"},
      {JavaScriptTokenType::BitAnd, "BitAnd"},
      {JavaScriptTokenType::BitXOr, "BitXOr"},
      {JavaScriptTokenType::BitOr, "BitOr"},
      {JavaScriptTokenType::And, "And"},
      {JavaScriptTokenType::Or, "Or"},
      {JavaScriptTokenType::MultiplyAssign, "MultiplyAssign"},
      {JavaScriptTokenType::DivideAssign, "DivideAssign"},
      {JavaScriptTokenType::ModulusAssign, "ModulusAssign"},
      {JavaScriptTokenType::PlusAssign, "PlusAssign"},
      {JavaScriptTokenType::MinusAssign, "MinusAssign"},
      {JavaScriptTokenType::LeftShiftArithmeticAssign, "LeftShiftArithmeticAssign"},
      {JavaScriptTokenType::RightShiftArithmeticAssign, "RightShiftArithmeticAssign"},
      {JavaScriptTokenType::RightShiftLogicalAssign, "RightShiftLogicalAssign"},
      {JavaScriptTokenType::BitAndAssign, "BitAndAssign"},
      {JavaScriptTokenType::BitXorAssign, "BitXorAssign"},
      {JavaScriptTokenType::BitOrAssign, "BitOrAssign"},
      {JavaScriptTokenType::NullLiteral, "NullLiteral"},
      {JavaScriptTokenType::BooleanLiteral, "BooleanLiteral"},
      {JavaScriptTokenType::DecimalLiteral, "DecimalLiteral"},
      {JavaScriptTokenType::HexIntegerLiteral, "HexIntegerLiteral"},
      {JavaScriptTokenType::OctalIntegerLiteral, "OctalIntegerLiteral"},
      {JavaScriptTokenType::Break, "Break"},
      {JavaScriptTokenType::Do, "Do"},
      {JavaScriptTokenType::Instanceof, "Instanceof"},
      {JavaScriptTokenType::Typeof, "Typeof"},
      {JavaScriptTokenType::Case, "Case"},
      {JavaScriptTokenType::Else, "Else"},
      {JavaScriptTokenType::New, "New"},
      {JavaScriptTokenType::Var, "Var"},
      {JavaScriptTokenType::Catch, "Catch"},
      {JavaScriptTokenType::Finally, "Finally"},
      {JavaScriptTokenType::Return, "Return"},
      {JavaScriptTokenType::Void, "Void"},
      {JavaScriptTokenType::Continue, "Continue"},
      {JavaScriptTokenType::For, "For"},
      {JavaScriptTokenType::Switch, "Switch"},
      {JavaScriptTokenType::While, "While"},
      {JavaScriptTokenType::Debugger, "Debugger"},
      {JavaScriptTokenType::Function, "Function"},
      {JavaScriptTokenType::This, "This"},
      {JavaScriptTokenType::With, "With"},
      {JavaScriptTokenType::Default, "Default"},
      {JavaScriptTokenType::If, "If"},
      {JavaScriptTokenType::Throw, "Throw"},
      {JavaScriptTokenType::Delete, "Delete"},
      {JavaScriptTokenType::In, "In"},
      {JavaScriptTokenType::Try, "Try"},
      {JavaScriptTokenType::Class, "Class"},
      {JavaScriptTokenType::Enum, "Enum"},
      {JavaScriptTokenType::Extends, "Extends"},
      {JavaScriptTokenType::Super, "Super"},
      {JavaScriptTokenType::Const, "Const"},
      {JavaScriptTokenType::Export, "Export"},
      {JavaScriptTokenType::Import, "Import"},
      {JavaScriptTokenType::Implements, "Implements"},
      {JavaScriptTokenType::Let, "Let"},
      {JavaScriptTokenType::Private, "Private"},
      {JavaScriptTokenType::Public, "Public"},
      {JavaScriptTokenType::Interface, "Interface"},
      {JavaScriptTokenType::Package, "Package"},
      {JavaScriptTokenType::Protected, "Protected"},
      {JavaScriptTokenType::Static, "Static"},
      {JavaScriptTokenType::Yield, "Yield"},
      {JavaScriptTokenType::Identifier, "Identifier"},
      {JavaScriptTokenType::StringLiteral, "StringLiteral"},
      {JavaScriptTokenType::WhiteSpaces, "WhiteSpaces"},
      {JavaScriptTokenType::MultiLineComment, "MultiLineComment"},
      {JavaScriptTokenType::SingleLineComment, "SingleLineComment"},
      {JavaScriptTokenType::UnexpectedCharacter, "UnexpectedCharacter"},
      {JavaScriptTokenType::Error, "Error"}};

  auto search = tokenTypeToStringMap.find(static_cast<JavaScriptTokenType>(type));
  if (search == tokenTypeToStringMap.end()) {
    throw std::runtime_error{"Undefined JavaScript Token type" + std::to_string(type)};
  }
  return search->second;
}

} // namespace injections
} // namespace protection
