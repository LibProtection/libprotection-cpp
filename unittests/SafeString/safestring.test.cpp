#include <catch/single_include/catch.hpp>
#include <SafeString.h>
#include <Languages/FilePath.h>
#include <Languages/Url.h>

using namespace protection::injections;

TEST_CASE("FilePath") {
  REQUIRE(R"(~\Assets\textFile.txt)" == SafeString<FilePath>::format(R"(~\Assets\{0})", "textFile.txt"));
  REQUIRE(R"(~\Assets\Новый документ.txt)" == SafeString<FilePath>::format(R"(~\Assets\{0})", "Новый документ.txt"));
  REQUIRE(R"(~\Assets\Новая директория\Новый документ.txt)" ==
          SafeString<FilePath>::format(R"(~\Assets\{1}\{0})", "Новый документ.txt", "Новая директория"));
}

TEST_CASE("Url") {
  REQUIRE("Assets/jsFile.js" == SafeString<Url>::format("{0}/{1}", "Assets", "jsFile.js"));
}
