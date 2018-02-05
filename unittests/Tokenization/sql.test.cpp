#include <catch/single_include/catch.hpp>
#include "protection/Single.h"
#include "protection/Languages.h"

using namespace protection::injections;

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
