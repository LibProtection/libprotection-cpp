#include <catch/single_include/catch.hpp>
#include "protection/Single.h"
#include "protection/Languages.h"

using namespace protection::injections;

TEST_CASE("JavaScript") {
  SECTION("JavaScript1") {
    std::string text{"var a = 1\n"
                     "var b = 2;\n"
                     "\n"
                     "if (c < a)\n"
                     "{\n"
                     "    func_call(a);\n"
                     "}"};
    auto tokens = Single<JavaScript>::instance().tokenize(text);

    REQUIRE(38 == tokens.size());

    REQUIRE("var" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(2 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Var) == tokens[0].tokenType);

    REQUIRE(" " == tokens[1].text);
    REQUIRE(3 == tokens[1].range.lowerBound);
    REQUIRE(3 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[1].tokenType);

    REQUIRE("a" == tokens[2].text);
    REQUIRE(4 == tokens[2].range.lowerBound);
    REQUIRE(4 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[2].tokenType);

    REQUIRE(" " == tokens[3].text);
    REQUIRE(5 == tokens[3].range.lowerBound);
    REQUIRE(5 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[3].tokenType);

    REQUIRE("=" == tokens[4].text);
    REQUIRE(6 == tokens[4].range.lowerBound);
    REQUIRE(6 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Assign) == tokens[4].tokenType);

    REQUIRE(" " == tokens[5].text);
    REQUIRE(7 == tokens[5].range.lowerBound);
    REQUIRE(7 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[5].tokenType);

    REQUIRE("1" == tokens[6].text);
    REQUIRE(8 == tokens[6].range.lowerBound);
    REQUIRE(8 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::DecimalLiteral) == tokens[6].tokenType);

    REQUIRE("\n" == tokens[7].text);
    REQUIRE(9 == tokens[7].range.lowerBound);
    REQUIRE(9 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[7].tokenType);

    REQUIRE("var" == tokens[8].text);
    REQUIRE(10 == tokens[8].range.lowerBound);
    REQUIRE(12 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Var) == tokens[8].tokenType);

    REQUIRE(" " == tokens[9].text);
    REQUIRE(13 == tokens[9].range.lowerBound);
    REQUIRE(13 == tokens[9].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[9].tokenType);

    REQUIRE("b" == tokens[10].text);
    REQUIRE(14 == tokens[10].range.lowerBound);
    REQUIRE(14 == tokens[10].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[10].tokenType);

    REQUIRE(" " == tokens[11].text);
    REQUIRE(15 == tokens[11].range.lowerBound);
    REQUIRE(15 == tokens[11].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[11].tokenType);

    REQUIRE("=" == tokens[12].text);
    REQUIRE(16 == tokens[12].range.lowerBound);
    REQUIRE(16 == tokens[12].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Assign) == tokens[12].tokenType);

    REQUIRE(" " == tokens[13].text);
    REQUIRE(17 == tokens[13].range.lowerBound);
    REQUIRE(17 == tokens[13].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[13].tokenType);

    REQUIRE("2" == tokens[14].text);
    REQUIRE(18 == tokens[14].range.lowerBound);
    REQUIRE(18 == tokens[14].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::DecimalLiteral) == tokens[14].tokenType);

    REQUIRE(";" == tokens[15].text);
    REQUIRE(19 == tokens[15].range.lowerBound);
    REQUIRE(19 == tokens[15].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::SemiColon) == tokens[15].tokenType);

    REQUIRE("\n" == tokens[16].text);
    REQUIRE(20 == tokens[16].range.lowerBound);
    REQUIRE(20 == tokens[16].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[16].tokenType);

    REQUIRE("\n" == tokens[17].text);
    REQUIRE(21 == tokens[17].range.lowerBound);
    REQUIRE(21 == tokens[17].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[17].tokenType);

    REQUIRE("if" == tokens[18].text);
    REQUIRE(22 == tokens[18].range.lowerBound);
    REQUIRE(23 == tokens[18].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::If) == tokens[18].tokenType);

    REQUIRE(" " == tokens[19].text);
    REQUIRE(24 == tokens[19].range.lowerBound);
    REQUIRE(24 == tokens[19].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[19].tokenType);

    REQUIRE("(" == tokens[20].text);
    REQUIRE(25 == tokens[20].range.lowerBound);
    REQUIRE(25 == tokens[20].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenParen) == tokens[20].tokenType);

    REQUIRE("c" == tokens[21].text);
    REQUIRE(26 == tokens[21].range.lowerBound);
    REQUIRE(26 == tokens[21].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[21].tokenType);

    REQUIRE(" " == tokens[22].text);
    REQUIRE(27 == tokens[22].range.lowerBound);
    REQUIRE(27 == tokens[22].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[22].tokenType);

    REQUIRE("<" == tokens[23].text);
    REQUIRE(28 == tokens[23].range.lowerBound);
    REQUIRE(28 == tokens[23].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LessThan) == tokens[23].tokenType);

    REQUIRE(" " == tokens[24].text);
    REQUIRE(29 == tokens[24].range.lowerBound);
    REQUIRE(29 == tokens[24].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[24].tokenType);

    REQUIRE("a" == tokens[25].text);
    REQUIRE(30 == tokens[25].range.lowerBound);
    REQUIRE(30 == tokens[25].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[25].tokenType);

    REQUIRE(")" == tokens[26].text);
    REQUIRE(31 == tokens[26].range.lowerBound);
    REQUIRE(31 == tokens[26].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseParen) == tokens[26].tokenType);

    REQUIRE("\n" == tokens[27].text);
    REQUIRE(32 == tokens[27].range.lowerBound);
    REQUIRE(32 == tokens[27].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[27].tokenType);

    REQUIRE("{" == tokens[28].text);
    REQUIRE(33 == tokens[28].range.lowerBound);
    REQUIRE(33 == tokens[28].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenBrace) == tokens[28].tokenType);

    REQUIRE("\n" == tokens[29].text);
    REQUIRE(34 == tokens[29].range.lowerBound);
    REQUIRE(34 == tokens[29].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[29].tokenType);

    REQUIRE("    " == tokens[30].text);
    REQUIRE(35 == tokens[30].range.lowerBound);
    REQUIRE(38 == tokens[30].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[30].tokenType);

    REQUIRE("func_call" == tokens[31].text);
    REQUIRE(39 == tokens[31].range.lowerBound);
    REQUIRE(47 == tokens[31].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[31].tokenType);

    REQUIRE("(" == tokens[32].text);
    REQUIRE(48 == tokens[32].range.lowerBound);
    REQUIRE(48 == tokens[32].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenParen) == tokens[32].tokenType);

    REQUIRE("a" == tokens[33].text);
    REQUIRE(49 == tokens[33].range.lowerBound);
    REQUIRE(49 == tokens[33].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[33].tokenType);

    REQUIRE(")" == tokens[34].text);
    REQUIRE(50 == tokens[34].range.lowerBound);
    REQUIRE(50 == tokens[34].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseParen) == tokens[34].tokenType);

    REQUIRE(";" == tokens[35].text);
    REQUIRE(51 == tokens[35].range.lowerBound);
    REQUIRE(51 == tokens[35].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::SemiColon) == tokens[35].tokenType);

    REQUIRE("\n" == tokens[36].text);
    REQUIRE(52 == tokens[36].range.lowerBound);
    REQUIRE(52 == tokens[36].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::LineTerminator) == tokens[36].tokenType);

    REQUIRE("}" == tokens[37].text);
    REQUIRE(53 == tokens[37].range.lowerBound);
    REQUIRE(53 == tokens[37].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseBrace) == tokens[37].tokenType);
  }

  SECTION("JavaScript2") {
    std::string text{"operationResult.innerText = 'Hello from internal JavaScript code!';"};
    auto tokens = Single<JavaScript>::instance().tokenize(text);

    REQUIRE(8 == tokens.size());

    REQUIRE("operationResult" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(14 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[0].tokenType);

    REQUIRE("." == tokens[1].text);
    REQUIRE(15 == tokens[1].range.lowerBound);
    REQUIRE(15 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Dot) == tokens[1].tokenType);

    REQUIRE("innerText" == tokens[2].text);
    REQUIRE(16 == tokens[2].range.lowerBound);
    REQUIRE(24 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[2].tokenType);

    REQUIRE(" " == tokens[3].text);
    REQUIRE(25 == tokens[3].range.lowerBound);
    REQUIRE(25 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[3].tokenType);

    REQUIRE("=" == tokens[4].text);
    REQUIRE(26 == tokens[4].range.lowerBound);
    REQUIRE(26 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Assign) == tokens[4].tokenType);

    REQUIRE(" " == tokens[5].text);
    REQUIRE(27 == tokens[5].range.lowerBound);
    REQUIRE(27 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[5].tokenType);

    REQUIRE("'Hello from internal JavaScript code!'" == tokens[6].text);
    REQUIRE(28 == tokens[6].range.lowerBound);
    REQUIRE(65 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::StringLiteral) == tokens[6].tokenType);

    REQUIRE(";" == tokens[7].text);
    REQUIRE(66 == tokens[7].range.lowerBound);
    REQUIRE(66 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::SemiColon) == tokens[7].tokenType);
  }
}
