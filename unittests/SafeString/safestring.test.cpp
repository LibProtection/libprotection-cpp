#include <catch/single_include/catch.hpp>
#include <SafeString.h>
#include <Languages.h>

using namespace protection::injections;

TEST_CASE("FilePath") {
  REQUIRE(R"(~\Assets\textFile.txt)" == SafeString<FilePath>::format(R"(~\Assets\{0})", "textFile.txt"));
  REQUIRE(R"(~\Assets\Новый документ.txt)" == SafeString<FilePath>::format(R"(~\Assets\{0})", "Новый документ.txt"));
  REQUIRE(R"(~\Assets\Новая директория\Новый документ.txt)" ==
          SafeString<FilePath>::format(R"(~\Assets\{1}\{0})", "Новый документ.txt", "Новая директория"));
}

TEST_CASE("Url") { REQUIRE("Assets/jsFile.js" == SafeString<Url>::format("{0}/{1}", "Assets", "jsFile.js")); }

TEST_CASE("Sql") {
  REQUIRE("SELECT * FROM myTable WHERE id = 1 AND myColumn = 'value1'" ==
          SafeString<Sql>::format("SELECT * FROM myTable WHERE id = {0} AND myColumn = '{1}'", 1, "value1"));
}

TEST_CASE("JavaScript") {
  REQUIRE("operationResult.innerText = 'Hello from internal JavaScript code!';" ==
          SafeString<Sql>::format("operationResult.innerText = '{0}';", "Hello from internal JavaScript code!"));
}

TEST_CASE("Html") {
  REQUIRE("<a href='Default.aspx' onclick='alert(\"Hello from embedded JavaScript code!\");"
          "return false'>This site&#39;s home page</a>" ==
          SafeString<Html>::format("<a href='{0}' onclick='alert(\"{1}\");return false'>{2}</a>", "Default.aspx",
                                   "Hello from embedded JavaScript code!", "This site's home page"));
}
