#include <catch/single_include/catch.hpp>
#include <Single.h>
#include <Languages.h>

using namespace protection::injections;

TEST_CASE("Url") {
  SECTION("https://user:password@host.domain/path/to/document/?a=1&b=2#fragment") {
    std::string text{"https://user:password@host.domain/path/to/document/?a=1&b=2#fragment"};
    auto tokens = Single<Url>::instance().tokenize(text);

    REQUIRE(25 == tokens.size());

    REQUIRE("https" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(4 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Scheme) == tokens[0].tokenType);

    REQUIRE(":" == tokens[1].text);
    REQUIRE(5 == tokens[1].range.lowerBound);
    REQUIRE(5 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("//" == tokens[2].text);
    REQUIRE(6 == tokens[2].range.lowerBound);
    REQUIRE(7 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[2].tokenType);

    REQUIRE("user" == tokens[3].text);
    REQUIRE(8 == tokens[3].range.lowerBound);
    REQUIRE(11 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::AuthorityEntry) == tokens[3].tokenType);

    REQUIRE(":" == tokens[4].text);
    REQUIRE(12 == tokens[4].range.lowerBound);
    REQUIRE(12 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[4].tokenType);

    REQUIRE("password" == tokens[5].text);
    REQUIRE(13 == tokens[5].range.lowerBound);
    REQUIRE(20 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::AuthorityEntry) == tokens[5].tokenType);

    REQUIRE("@" == tokens[6].text);
    REQUIRE(21 == tokens[6].range.lowerBound);
    REQUIRE(21 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[6].tokenType);

    REQUIRE("host.domain" == tokens[7].text);
    REQUIRE(22 == tokens[7].range.lowerBound);
    REQUIRE(32 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::AuthorityEntry) == tokens[7].tokenType);

    REQUIRE("/" == tokens[8].text);
    REQUIRE(33 == tokens[8].range.lowerBound);
    REQUIRE(33 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[8].tokenType);

    REQUIRE("path" == tokens[9].text);
    REQUIRE(34 == tokens[9].range.lowerBound);
    REQUIRE(37 == tokens[9].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[9].tokenType);

    REQUIRE("/" == tokens[10].text);
    REQUIRE(38 == tokens[10].range.lowerBound);
    REQUIRE(38 == tokens[10].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[10].tokenType);

    REQUIRE("to" == tokens[11].text);
    REQUIRE(39 == tokens[11].range.lowerBound);
    REQUIRE(40 == tokens[11].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[11].tokenType);

    REQUIRE("/" == tokens[12].text);
    REQUIRE(41 == tokens[12].range.lowerBound);
    REQUIRE(41 == tokens[12].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[12].tokenType);

    REQUIRE("document" == tokens[13].text);
    REQUIRE(42 == tokens[13].range.lowerBound);
    REQUIRE(49 == tokens[13].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[13].tokenType);

    REQUIRE("/" == tokens[14].text);
    REQUIRE(50 == tokens[14].range.lowerBound);
    REQUIRE(50 == tokens[14].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[14].tokenType);

    REQUIRE("?" == tokens[15].text);
    REQUIRE(51 == tokens[15].range.lowerBound);
    REQUIRE(51 == tokens[15].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[15].tokenType);

    REQUIRE("a" == tokens[16].text);
    REQUIRE(52 == tokens[16].range.lowerBound);
    REQUIRE(52 == tokens[16].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[16].tokenType);

    REQUIRE("=" == tokens[17].text);
    REQUIRE(53 == tokens[17].range.lowerBound);
    REQUIRE(53 == tokens[17].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[17].tokenType);

    REQUIRE("1" == tokens[18].text);
    REQUIRE(54 == tokens[18].range.lowerBound);
    REQUIRE(54 == tokens[18].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[18].tokenType);

    REQUIRE("&" == tokens[19].text);
    REQUIRE(55 == tokens[19].range.lowerBound);
    REQUIRE(55 == tokens[19].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[19].tokenType);

    REQUIRE("b" == tokens[20].text);
    REQUIRE(56 == tokens[20].range.lowerBound);
    REQUIRE(56 == tokens[20].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[20].tokenType);

    REQUIRE("=" == tokens[21].text);
    REQUIRE(57 == tokens[21].range.lowerBound);
    REQUIRE(57 == tokens[21].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[21].tokenType);

    REQUIRE("2" == tokens[22].text);
    REQUIRE(58 == tokens[22].range.lowerBound);
    REQUIRE(58 == tokens[22].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[22].tokenType);

    REQUIRE("#" == tokens[23].text);
    REQUIRE(59 == tokens[23].range.lowerBound);
    REQUIRE(59 == tokens[23].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[23].tokenType);

    REQUIRE("fragment" == tokens[24].text);
    REQUIRE(60 == tokens[24].range.lowerBound);
    REQUIRE(67 == tokens[24].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Fragment) == tokens[24].tokenType);
  }

  SECTION("Assets/jsFile.js") {
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
}