#include <catch/single_include/catch.hpp>
#include <Single.h>
#include <Languages.h>

using namespace protection::injections;

TEST_CASE("FilePath") {
  std::string text{R"(~\Assets\textFile.txt)"};
  auto tokens = Single<FilePath>::instance().tokenize(text);

  REQUIRE(5 == tokens.size());

  REQUIRE("~" == tokens[0].text);
  REQUIRE(0 == tokens[0].range.lowerBound);
  REQUIRE(0 == tokens[0].range.upperBound);
  REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[0].tokenType);

  REQUIRE("\\" == tokens[1].text);
  REQUIRE(1 == tokens[1].range.lowerBound);
  REQUIRE(1 == tokens[1].range.upperBound);
  REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

  REQUIRE("Assets" == tokens[2].text);
  REQUIRE(2 == tokens[2].range.lowerBound);
  REQUIRE(7 == tokens[2].range.upperBound);
  REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

  REQUIRE("\\" == tokens[3].text);
  REQUIRE(8 == tokens[3].range.lowerBound);
  REQUIRE(8 == tokens[3].range.upperBound);
  REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[3].tokenType);

  REQUIRE("textFile.txt" == tokens[4].text);
  REQUIRE(9 == tokens[4].range.lowerBound);
  REQUIRE(20 == tokens[4].range.upperBound);
  REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[4].tokenType);
}

TEST_CASE("Url") {
  std::string text{"Assets/jsFile.js"};
  auto tokens = Single<Url>::instance().tokenize(text);

  REQUIRE(3 == tokens.size());

  REQUIRE("Assets" == tokens[0].text);
  REQUIRE(0 == tokens[0].range.lowerBound);
  REQUIRE(5 == tokens[0].range.upperBound);
  REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[0].tokenType);

  REQUIRE("/" == tokens[1].text);
  REQUIRE(6 == tokens[1].range.lowerBound);
  REQUIRE(6 == tokens[1].range.upperBound);
  REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[1].tokenType);

  REQUIRE("jsFile.js" == tokens[2].text);
  REQUIRE(7 == tokens[2].range.lowerBound);
  REQUIRE(15 == tokens[2].range.upperBound);
  REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[2].tokenType);
}

TEST_CASE("Sql") {
  std::string text{"SELECT * FROM myTable WHERE id = 1 AND myColumn = 'value1'"};
  auto tokens = Single<Sql>::instance().tokenize(text);

  REQUIRE(23 == tokens.size());

  REQUIRE("SELECT" == tokens[0].text);
  REQUIRE(0 == tokens[0].range.lowerBound);
  REQUIRE(5 == tokens[0].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Select) == tokens[0].tokenType);

  REQUIRE(" " == tokens[1].text);
  REQUIRE(6 == tokens[1].range.lowerBound);
  REQUIRE(6 == tokens[1].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[1].tokenType);

  REQUIRE("*" == tokens[2].text);
  REQUIRE(7 == tokens[2].range.lowerBound);
  REQUIRE(7 == tokens[2].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::StarSymbol) == tokens[2].tokenType);

  REQUIRE(" " == tokens[3].text);
  REQUIRE(8 == tokens[3].range.lowerBound);
  REQUIRE(8 == tokens[3].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[3].tokenType);

  REQUIRE("FROM" == tokens[4].text);
  REQUIRE(9 == tokens[4].range.lowerBound);
  REQUIRE(12 == tokens[4].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::From) == tokens[4].tokenType);

  REQUIRE(" " == tokens[5].text);
  REQUIRE(13 == tokens[5].range.lowerBound);
  REQUIRE(13 == tokens[5].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[5].tokenType);

  REQUIRE("myTable" == tokens[6].text);
  REQUIRE(14 == tokens[6].range.lowerBound);
  REQUIRE(20 == tokens[6].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Id) == tokens[6].tokenType);

  REQUIRE(" " == tokens[7].text);
  REQUIRE(21 == tokens[7].range.lowerBound);
  REQUIRE(21 == tokens[7].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[7].tokenType);

  REQUIRE("WHERE" == tokens[8].text);
  REQUIRE(22 == tokens[8].range.lowerBound);
  REQUIRE(26 == tokens[8].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Where) == tokens[8].tokenType);

  REQUIRE(" " == tokens[9].text);
  REQUIRE(27 == tokens[9].range.lowerBound);
  REQUIRE(27 == tokens[9].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[9].tokenType);

  REQUIRE("id" == tokens[10].text);
  REQUIRE(28 == tokens[10].range.lowerBound);
  REQUIRE(29 == tokens[10].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Id) == tokens[10].tokenType);

  REQUIRE(" " == tokens[11].text);
  REQUIRE(30 == tokens[11].range.lowerBound);
  REQUIRE(30 == tokens[11].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[11].tokenType);

  REQUIRE("=" == tokens[12].text);
  REQUIRE(31 == tokens[12].range.lowerBound);
  REQUIRE(31 == tokens[12].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::EqualSymbol) == tokens[12].tokenType);

  REQUIRE(" " == tokens[13].text);
  REQUIRE(32 == tokens[13].range.lowerBound);
  REQUIRE(32 == tokens[13].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[13].tokenType);

  REQUIRE("1" == tokens[14].text);
  REQUIRE(33 == tokens[14].range.lowerBound);
  REQUIRE(33 == tokens[14].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::DecimalLiteral) == tokens[14].tokenType);

  REQUIRE(" " == tokens[15].text);
  REQUIRE(34 == tokens[15].range.lowerBound);
  REQUIRE(34 == tokens[15].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[15].tokenType);

  REQUIRE("AND" == tokens[16].text);
  REQUIRE(35 == tokens[16].range.lowerBound);
  REQUIRE(37 == tokens[16].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::And) == tokens[16].tokenType);

  REQUIRE(" " == tokens[17].text);
  REQUIRE(38 == tokens[17].range.lowerBound);
  REQUIRE(38 == tokens[17].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[17].tokenType);

  REQUIRE("myColumn" == tokens[18].text);
  REQUIRE(39 == tokens[18].range.lowerBound);
  REQUIRE(46 == tokens[18].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Id) == tokens[18].tokenType);

  REQUIRE(" " == tokens[19].text);
  REQUIRE(47 == tokens[19].range.lowerBound);
  REQUIRE(47 == tokens[19].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[19].tokenType);

  REQUIRE("=" == tokens[20].text);
  REQUIRE(48 == tokens[20].range.lowerBound);
  REQUIRE(48 == tokens[20].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::EqualSymbol) == tokens[20].tokenType);

  REQUIRE(" " == tokens[21].text);
  REQUIRE(49 == tokens[21].range.lowerBound);
  REQUIRE(49 == tokens[21].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::Space) == tokens[21].tokenType);

  REQUIRE("'value1'" == tokens[22].text);
  REQUIRE(50 == tokens[22].range.lowerBound);
  REQUIRE(57 == tokens[22].range.upperBound);
  REQUIRE(TOKEN_TYPE(SqlTokenType::StringLiteral) == tokens[22].tokenType);
}

TEST_CASE("JavaScript") {
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

TEST_CASE("Html") {
  std::string text{"<a href='Default.aspx' onclick='alert(\"Hello from embedded JavaScript code!\");"
                   "return false'>This site's home page</a>"};
  auto tokens = Single<Html>::instance().tokenize(text);

  REQUIRE(18 == tokens.size());

  REQUIRE("<a" == tokens[0].text);
  REQUIRE(0 == tokens[0].range.lowerBound);
  REQUIRE(1 == tokens[0].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[0].tokenType);

  REQUIRE("href" == tokens[1].text);
  REQUIRE(3 == tokens[1].range.lowerBound);
  REQUIRE(6 == tokens[1].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::AttributeName) == tokens[1].tokenType);

  REQUIRE("=" == tokens[2].text);
  REQUIRE(7 == tokens[2].range.lowerBound);
  REQUIRE(7 == tokens[2].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagEquals) == tokens[2].tokenType);

  REQUIRE("Default.aspx" == tokens[3].text);
  REQUIRE(9 == tokens[3].range.lowerBound);
  REQUIRE(20 == tokens[3].range.upperBound);
  REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[3].tokenType);

  REQUIRE("onclick" == tokens[4].text);
  REQUIRE(23 == tokens[4].range.lowerBound);
  REQUIRE(29 == tokens[4].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::AttributeName) == tokens[4].tokenType);

  REQUIRE("=" == tokens[5].text);
  REQUIRE(30 == tokens[5].range.lowerBound);
  REQUIRE(30 == tokens[5].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagEquals) == tokens[5].tokenType);

  REQUIRE("alert" == tokens[6].text);
  REQUIRE(32 == tokens[6].range.lowerBound);
  REQUIRE(36 == tokens[6].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[6].tokenType);

  REQUIRE("(" == tokens[7].text);
  REQUIRE(37 == tokens[7].range.lowerBound);
  REQUIRE(37 == tokens[7].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenParen) == tokens[7].tokenType);

  REQUIRE("\"Hello from embedded JavaScript code!\"" == tokens[8].text);
  REQUIRE(38 == tokens[8].range.lowerBound);
  REQUIRE(75 == tokens[8].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::StringLiteral) == tokens[8].tokenType);

  REQUIRE(")" == tokens[9].text);
  REQUIRE(76 == tokens[9].range.lowerBound);
  REQUIRE(76 == tokens[9].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseParen) == tokens[9].tokenType);

  REQUIRE(";" == tokens[10].text);
  REQUIRE(77 == tokens[10].range.lowerBound);
  REQUIRE(77 == tokens[10].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::SemiColon) == tokens[10].tokenType);

  REQUIRE("return" == tokens[11].text);
  REQUIRE(78 == tokens[11].range.lowerBound);
  REQUIRE(83 == tokens[11].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Return) == tokens[11].tokenType);

  REQUIRE(" " == tokens[12].text);
  REQUIRE(84 == tokens[12].range.lowerBound);
  REQUIRE(84 == tokens[12].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::WhiteSpaces) == tokens[12].tokenType);

  REQUIRE("false" == tokens[13].text);
  REQUIRE(85 == tokens[13].range.lowerBound);
  REQUIRE(89 == tokens[13].range.upperBound);
  REQUIRE(TOKEN_TYPE(JavaScriptTokenType::BooleanLiteral) == tokens[13].tokenType);

  REQUIRE(">" == tokens[14].text);
  REQUIRE(91 == tokens[14].range.lowerBound);
  REQUIRE(91 == tokens[14].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[14].tokenType);

  REQUIRE("This site's home page" == tokens[15].text);
  REQUIRE(92 == tokens[15].range.lowerBound);
  REQUIRE(112 == tokens[15].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[15].tokenType);

  REQUIRE("</a" == tokens[16].text);
  REQUIRE(113 == tokens[16].range.lowerBound);
  REQUIRE(115 == tokens[16].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[16].tokenType);

  REQUIRE(">" == tokens[17].text);
  REQUIRE(116 == tokens[17].range.lowerBound);
  REQUIRE(116 == tokens[17].range.upperBound);
  REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[17].tokenType);
}
