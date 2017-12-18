#include "Languages/Sql.h"
#include "SQLLexer.h"

namespace protection {

std::pair<std::string, bool> Sql::trySanitize(const std::string &text, Token context) const {
  switch (context.languageProviderType) {
  case LanguageProviderType::Sql: {
    auto encodeResult = trySqlEncode(text, (SqlTokenType)context.tokenType);
    if (encodeResult.second) {
      return encodeResult;
    }
    break;
  }
  default:
    throw std::runtime_error{"Unsupported SQL island: " + context.toString()};
  }

  return {{}, false};
}

std::pair<std::string, bool> Sql::trySqlEncode(const std::string &text, SqlTokenType tokenType) const {
  switch (tokenType) {
  case SqlTokenType::StringLiteral: {
    auto replaceAll = [](std::string &str, const std::string &search, const std::string &replace) {
      std::string::size_type pos = 0;
      while ((pos = str.find(search, pos)) != std::string::npos) {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
      }
    };

    auto encoded = text;
    replaceAll(encoded, "''", "'");
    replaceAll(encoded, "'", "''");

    return {encoded, true};
  }
  default:
    break;
  }

  return {{}, false};
}

Token Sql::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(LanguageProviderType::Sql, type, lowerBound, upperBound, text, isTrivial(type, text));
}

bool Sql::isTrivial(TokenType type, const std::string &text) const {
  switch (static_cast<SqlTokenType>(type)) {
  case SqlTokenType::Space:
  case SqlTokenType::CommentInput:
  case SqlTokenType::LineComment:
  case SqlTokenType::NullLiteral:
  case SqlTokenType::FilesizeLiteral:
  case SqlTokenType::StartNationalStringLiteral:
  case SqlTokenType::StringLiteral:
  case SqlTokenType::DecimalLiteral:
  case SqlTokenType::HexadecimalLiteral:
  case SqlTokenType::RealLiteral:
  case SqlTokenType::NullSpecLiteral:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<antlr4::Lexer> Sql::createLexer(const std::string &text) const {
  return std::unique_ptr<sql::SQLLexer>(new sql::SQLLexer(new antlr4::ANTLRInputStream(text)));
}

TokenType Sql::convertAntlrTokenType(size_t antlrTokenType) const { return antlrTokenType; }

} // namespace protection