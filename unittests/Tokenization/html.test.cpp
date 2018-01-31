#include <catch/single_include/catch.hpp>
#include <Single.h>
#include <Languages.h>

using namespace protection::injections;

TEST_CASE("Html") {

  SECTION("general") {
    std::string text{"<html>\n"
                     "  <body>\n"
                     "    <script>alert('script')</script>\n"
                     "    <a href=\"http://host.domain/path/to/file.exension?query1=param1&query2=param2#fragment-id\" "
                     "onclick=\"alert('click')\">Link</a>\n"
                     "  </body>\n"
                     "</html>"};

    auto tokens = Single<Html>::instance().tokenize(text);
    REQUIRE(54 == tokens.size());

    REQUIRE("<html" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(4 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[0].tokenType);

    REQUIRE(">" == tokens[1].text);
    REQUIRE(5 == tokens[1].range.lowerBound);
    REQUIRE(5 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[1].tokenType);

    REQUIRE("\n  " == tokens[2].text);
    REQUIRE(6 == tokens[2].range.lowerBound);
    REQUIRE(8 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[2].tokenType);

    REQUIRE("<body" == tokens[3].text);
    REQUIRE(9 == tokens[3].range.lowerBound);
    REQUIRE(13 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[3].tokenType);

    REQUIRE(">" == tokens[4].text);
    REQUIRE(14 == tokens[4].range.lowerBound);
    REQUIRE(14 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[4].tokenType);

    REQUIRE("\n    " == tokens[5].text);
    REQUIRE(15 == tokens[5].range.lowerBound);
    REQUIRE(19 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[5].tokenType);

    REQUIRE("<script" == tokens[6].text);
    REQUIRE(20 == tokens[6].range.lowerBound);
    REQUIRE(26 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[6].tokenType);

    REQUIRE(">" == tokens[7].text);
    REQUIRE(27 == tokens[7].range.lowerBound);
    REQUIRE(27 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[7].tokenType);

    REQUIRE("alert" == tokens[8].text);
    REQUIRE(28 == tokens[8].range.lowerBound);
    REQUIRE(32 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[8].tokenType);

    REQUIRE("(" == tokens[9].text);
    REQUIRE(33 == tokens[9].range.lowerBound);
    REQUIRE(33 == tokens[9].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenParen) == tokens[9].tokenType);

    REQUIRE("'script'" == tokens[10].text);
    REQUIRE(34 == tokens[10].range.lowerBound);
    REQUIRE(41 == tokens[10].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::StringLiteral) == tokens[10].tokenType);

    REQUIRE(")" == tokens[11].text);
    REQUIRE(42 == tokens[11].range.lowerBound);
    REQUIRE(42 == tokens[11].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseParen) == tokens[11].tokenType);

    REQUIRE("</script" == tokens[12].text);
    REQUIRE(43 == tokens[12].range.lowerBound);
    REQUIRE(50 == tokens[12].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[12].tokenType);

    REQUIRE(">" == tokens[13].text);
    REQUIRE(51 == tokens[13].range.lowerBound);
    REQUIRE(51 == tokens[13].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[13].tokenType);

    REQUIRE("\n    " == tokens[14].text);
    REQUIRE(52 == tokens[14].range.lowerBound);
    REQUIRE(56 == tokens[14].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[14].tokenType);

    REQUIRE("<a" == tokens[15].text);
    REQUIRE(57 == tokens[15].range.lowerBound);
    REQUIRE(58 == tokens[15].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[15].tokenType);

    REQUIRE("href" == tokens[16].text);
    REQUIRE(60 == tokens[16].range.lowerBound);
    REQUIRE(63 == tokens[16].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::AttributeName) == tokens[16].tokenType);

    REQUIRE("=" == tokens[17].text);
    REQUIRE(64 == tokens[17].range.lowerBound);
    REQUIRE(64 == tokens[17].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagEquals) == tokens[17].tokenType);

    REQUIRE("http" == tokens[18].text);
    REQUIRE(66 == tokens[18].range.lowerBound);
    REQUIRE(69 == tokens[18].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Scheme) == tokens[18].tokenType);

    REQUIRE(":" == tokens[19].text);
    REQUIRE(70 == tokens[19].range.lowerBound);
    REQUIRE(70 == tokens[19].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[19].tokenType);

    REQUIRE("//" == tokens[20].text);
    REQUIRE(71 == tokens[20].range.lowerBound);
    REQUIRE(72 == tokens[20].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[20].tokenType);

    REQUIRE("host.domain" == tokens[21].text);
    REQUIRE(73 == tokens[21].range.lowerBound);
    REQUIRE(83 == tokens[21].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::AuthorityEntry) == tokens[21].tokenType);

    REQUIRE("/" == tokens[22].text);
    REQUIRE(84 == tokens[22].range.lowerBound);
    REQUIRE(84 == tokens[22].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[22].tokenType);

    REQUIRE("path" == tokens[23].text);
    REQUIRE(85 == tokens[23].range.lowerBound);
    REQUIRE(88 == tokens[23].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[23].tokenType);

    REQUIRE("/" == tokens[24].text);
    REQUIRE(89 == tokens[24].range.lowerBound);
    REQUIRE(89 == tokens[24].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[24].tokenType);

    REQUIRE("to" == tokens[25].text);
    REQUIRE(90 == tokens[25].range.lowerBound);
    REQUIRE(91 == tokens[25].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[25].tokenType);

    REQUIRE("/" == tokens[26].text);
    REQUIRE(92 == tokens[26].range.lowerBound);
    REQUIRE(92 == tokens[26].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[26].tokenType);

    REQUIRE("file.exension" == tokens[27].text);
    REQUIRE(93 == tokens[27].range.lowerBound);
    REQUIRE(105 == tokens[27].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::PathEntry) == tokens[27].tokenType);

    REQUIRE("?" == tokens[28].text);
    REQUIRE(106 == tokens[28].range.lowerBound);
    REQUIRE(106 == tokens[28].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[28].tokenType);

    REQUIRE("query1" == tokens[29].text);
    REQUIRE(107 == tokens[29].range.lowerBound);
    REQUIRE(112 == tokens[29].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[29].tokenType);

    REQUIRE("=" == tokens[30].text);
    REQUIRE(113 == tokens[30].range.lowerBound);
    REQUIRE(113 == tokens[30].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[30].tokenType);

    REQUIRE("param1" == tokens[31].text);
    REQUIRE(114 == tokens[31].range.lowerBound);
    REQUIRE(119 == tokens[31].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[31].tokenType);

    REQUIRE("&" == tokens[32].text);
    REQUIRE(120 == tokens[32].range.lowerBound);
    REQUIRE(120 == tokens[32].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[32].tokenType);

    REQUIRE("query2" == tokens[33].text);
    REQUIRE(121 == tokens[33].range.lowerBound);
    REQUIRE(126 == tokens[33].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[33].tokenType);

    REQUIRE("=" == tokens[34].text);
    REQUIRE(127 == tokens[34].range.lowerBound);
    REQUIRE(127 == tokens[34].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[34].tokenType);

    REQUIRE("param2" == tokens[35].text);
    REQUIRE(128 == tokens[35].range.lowerBound);
    REQUIRE(133 == tokens[35].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::QueryEntry) == tokens[35].tokenType);

    REQUIRE("#" == tokens[36].text);
    REQUIRE(134 == tokens[36].range.lowerBound);
    REQUIRE(134 == tokens[36].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Separator) == tokens[36].tokenType);

    REQUIRE("fragment-id" == tokens[37].text);
    REQUIRE(135 == tokens[37].range.lowerBound);
    REQUIRE(145 == tokens[37].range.upperBound);
    REQUIRE(TOKEN_TYPE(UrlTokenType::Fragment) == tokens[37].tokenType);

    REQUIRE("onclick" == tokens[38].text);
    REQUIRE(148 == tokens[38].range.lowerBound);
    REQUIRE(154 == tokens[38].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::AttributeName) == tokens[38].tokenType);

    REQUIRE("=" == tokens[39].text);
    REQUIRE(155 == tokens[39].range.lowerBound);
    REQUIRE(155 == tokens[39].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagEquals) == tokens[39].tokenType);

    REQUIRE("alert" == tokens[40].text);
    REQUIRE(157 == tokens[40].range.lowerBound);
    REQUIRE(161 == tokens[40].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::Identifier) == tokens[40].tokenType);

    REQUIRE("(" == tokens[41].text);
    REQUIRE(162 == tokens[41].range.lowerBound);
    REQUIRE(162 == tokens[41].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::OpenParen) == tokens[41].tokenType);

    REQUIRE("'click'" == tokens[42].text);
    REQUIRE(163 == tokens[42].range.lowerBound);
    REQUIRE(169 == tokens[42].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::StringLiteral) == tokens[42].tokenType);

    REQUIRE(")" == tokens[43].text);
    REQUIRE(170 == tokens[43].range.lowerBound);
    REQUIRE(170 == tokens[43].range.upperBound);
    REQUIRE(TOKEN_TYPE(JavaScriptTokenType::CloseParen) == tokens[43].tokenType);

    REQUIRE(">" == tokens[44].text);
    REQUIRE(172 == tokens[44].range.lowerBound);
    REQUIRE(172 == tokens[44].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[44].tokenType);

    REQUIRE("Link" == tokens[45].text);
    REQUIRE(173 == tokens[45].range.lowerBound);
    REQUIRE(176 == tokens[45].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[45].tokenType);

    REQUIRE("</a" == tokens[46].text);
    REQUIRE(177 == tokens[46].range.lowerBound);
    REQUIRE(179 == tokens[46].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[46].tokenType);

    REQUIRE(">" == tokens[47].text);
    REQUIRE(180 == tokens[47].range.lowerBound);
    REQUIRE(180 == tokens[47].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[47].tokenType);

    REQUIRE("\n  " == tokens[48].text);
    REQUIRE(181 == tokens[48].range.lowerBound);
    REQUIRE(183 == tokens[48].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[48].tokenType);

    REQUIRE("</body" == tokens[49].text);
    REQUIRE(184 == tokens[49].range.lowerBound);
    REQUIRE(189 == tokens[49].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[49].tokenType);

    REQUIRE(">" == tokens[50].text);
    REQUIRE(190 == tokens[50].range.lowerBound);
    REQUIRE(190 == tokens[50].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[50].tokenType);

    REQUIRE("\n" == tokens[51].text);
    REQUIRE(191 == tokens[51].range.lowerBound);
    REQUIRE(191 == tokens[51].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::HtmlText) == tokens[51].tokenType);

    REQUIRE("</html" == tokens[52].text);
    REQUIRE(192 == tokens[52].range.lowerBound);
    REQUIRE(197 == tokens[52].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagOpen) == tokens[52].tokenType);

    REQUIRE(">" == tokens[53].text);
    REQUIRE(198 == tokens[53].range.lowerBound);
    REQUIRE(198 == tokens[53].range.upperBound);
    REQUIRE(TOKEN_TYPE(HtmlTokenType::TagClose) == tokens[53].tokenType);
  }

  SECTION("general") {
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
}
