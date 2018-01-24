#include <catch/single_include/catch.hpp>
#include <SafeString.h>
#include <Languages/FilePath.h>

using namespace protection::injections;

TEST_CASE("FilePath") {
  REQUIRE(R"(~\Assets\textFile.txt)" == SafeString<FilePath>::format(R"(~\Assets\{0})", "textFile.txt"));
}
