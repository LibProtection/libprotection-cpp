#include <catch/single_include/catch.hpp>
#include "protection/Single.h"
#include "protection/Languages.h"

using namespace protection::injections;

template <typename T, size_t N> std::string ConstructStr(T (&arr)[N]) { return std::string{arr, N - 1}; }

TEST_CASE("FilePath") {
  SECTION("general") {
    std::string text{R"(C:\Windows\System32\calc.exe)"};
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(7 == tokens.size());

    REQUIRE("C:" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[0].tokenType);

    REQUIRE("\\" == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("Windows" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(9 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

    REQUIRE("\\" == tokens[3].text);
    REQUIRE(10 == tokens[3].range.lowerBound);
    REQUIRE(10 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[3].tokenType);

    REQUIRE("System32" == tokens[4].text);
    REQUIRE(11 == tokens[4].range.lowerBound);
    REQUIRE(18 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[4].tokenType);

    REQUIRE("\\" == tokens[5].text);
    REQUIRE(19 == tokens[5].range.lowerBound);
    REQUIRE(19 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[5].tokenType);

    REQUIRE("calc.exe" == tokens[6].text);
    REQUIRE(20 == tokens[6].range.lowerBound);
    REQUIRE(27 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[6].tokenType);
  }

  SECTION("general1") {
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

  SECTION("ntfs-attributes") {
    std::string text{R"(C:\Windows:$I30:$INDEX_ALLOCATION\System32\calc.exe)"};
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(9 == tokens.size());

    REQUIRE("C:" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[0].tokenType);

    REQUIRE("\\" == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("Windows" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(9 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

    REQUIRE(":$I30" == tokens[3].text);
    REQUIRE(10 == tokens[3].range.lowerBound);
    REQUIRE(14 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::NTFSAttribute) == tokens[3].tokenType);

    REQUIRE(":$INDEX_ALLOCATION" == tokens[4].text);
    REQUIRE(15 == tokens[4].range.lowerBound);
    REQUIRE(32 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::NTFSAttribute) == tokens[4].tokenType);

    REQUIRE("\\" == tokens[5].text);
    REQUIRE(33 == tokens[5].range.lowerBound);
    REQUIRE(33 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[5].tokenType);

    REQUIRE("System32" == tokens[6].text);
    REQUIRE(34 == tokens[6].range.lowerBound);
    REQUIRE(41 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[6].tokenType);

    REQUIRE("\\" == tokens[7].text);
    REQUIRE(42 == tokens[7].range.lowerBound);
    REQUIRE(42 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[7].tokenType);

    REQUIRE("calc.exe" == tokens[8].text);
    REQUIRE(43 == tokens[8].range.lowerBound);
    REQUIRE(50 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[8].tokenType);
  }

  SECTION("nullbyte") {

    auto text = ConstructStr("C:\\Windows\\System32\\\0calc.exe");
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(8 == tokens.size());

    REQUIRE("C:" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[0].tokenType);

    REQUIRE("\\" == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("Windows" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(9 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

    REQUIRE("\\" == tokens[3].text);
    REQUIRE(10 == tokens[3].range.lowerBound);
    REQUIRE(10 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[3].tokenType);

    REQUIRE("System32" == tokens[4].text);
    REQUIRE(11 == tokens[4].range.lowerBound);
    REQUIRE(18 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[4].tokenType);

    REQUIRE("\\" == tokens[5].text);
    REQUIRE(19 == tokens[5].range.lowerBound);
    REQUIRE(19 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[5].tokenType);

    REQUIRE(ConstructStr("\0") == tokens[6].text);
    REQUIRE(20 == tokens[6].range.lowerBound);
    REQUIRE(20 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DisallowedSymbol) == tokens[6].tokenType);

    REQUIRE("calc.exe" == tokens[7].text);
    REQUIRE(21 == tokens[7].range.lowerBound);
    REQUIRE(28 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[7].tokenType);
  }

  SECTION("slashes") {
    std::string text{R"(C:/Windows\System32/calc.exe)"};
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(7 == tokens.size());

    REQUIRE("C:" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[0].tokenType);

    REQUIRE("/" == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("Windows" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(9 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

    REQUIRE("\\" == tokens[3].text);
    REQUIRE(10 == tokens[3].range.lowerBound);
    REQUIRE(10 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[3].tokenType);

    REQUIRE("System32" == tokens[4].text);
    REQUIRE(11 == tokens[4].range.lowerBound);
    REQUIRE(18 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[4].tokenType);

    REQUIRE("/" == tokens[5].text);
    REQUIRE(19 == tokens[5].range.lowerBound);
    REQUIRE(19 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[5].tokenType);

    REQUIRE("calc.exe" == tokens[6].text);
    REQUIRE(20 == tokens[6].range.lowerBound);
    REQUIRE(27 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[6].tokenType);
  }

  SECTION("traversal") {
    std::string text{R"(C:\Windows\System32\a\b\c\..\..\..\calc.exe)"};
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(19 == tokens.size());

    REQUIRE("C:" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[0].tokenType);

    REQUIRE("\\" == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[1].tokenType);

    REQUIRE("Windows" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(9 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[2].tokenType);

    REQUIRE("\\" == tokens[3].text);
    REQUIRE(10 == tokens[3].range.lowerBound);
    REQUIRE(10 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[3].tokenType);

    REQUIRE("System32" == tokens[4].text);
    REQUIRE(11 == tokens[4].range.lowerBound);
    REQUIRE(18 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[4].tokenType);

    REQUIRE("\\" == tokens[5].text);
    REQUIRE(19 == tokens[5].range.lowerBound);
    REQUIRE(19 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[5].tokenType);

    REQUIRE("a" == tokens[6].text);
    REQUIRE(20 == tokens[6].range.lowerBound);
    REQUIRE(20 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[6].tokenType);

    REQUIRE("\\" == tokens[7].text);
    REQUIRE(21 == tokens[7].range.lowerBound);
    REQUIRE(21 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[7].tokenType);

    REQUIRE("b" == tokens[8].text);
    REQUIRE(22 == tokens[8].range.lowerBound);
    REQUIRE(22 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[8].tokenType);

    REQUIRE("\\" == tokens[9].text);
    REQUIRE(23 == tokens[9].range.lowerBound);
    REQUIRE(23 == tokens[9].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[9].tokenType);

    REQUIRE("c" == tokens[10].text);
    REQUIRE(24 == tokens[10].range.lowerBound);
    REQUIRE(24 == tokens[10].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[10].tokenType);

    REQUIRE("\\" == tokens[11].text);
    REQUIRE(25 == tokens[11].range.lowerBound);
    REQUIRE(25 == tokens[11].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[11].tokenType);

    REQUIRE(".." == tokens[12].text);
    REQUIRE(26 == tokens[12].range.lowerBound);
    REQUIRE(27 == tokens[12].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[12].tokenType);

    REQUIRE("\\" == tokens[13].text);
    REQUIRE(28 == tokens[13].range.lowerBound);
    REQUIRE(28 == tokens[13].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[13].tokenType);

    REQUIRE(".." == tokens[14].text);
    REQUIRE(29 == tokens[14].range.lowerBound);
    REQUIRE(30 == tokens[14].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[14].tokenType);

    REQUIRE("\\" == tokens[15].text);
    REQUIRE(31 == tokens[15].range.lowerBound);
    REQUIRE(31 == tokens[15].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[15].tokenType);

    REQUIRE(".." == tokens[16].text);
    REQUIRE(32 == tokens[16].range.lowerBound);
    REQUIRE(33 == tokens[16].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[16].tokenType);

    REQUIRE("\\" == tokens[17].text);
    REQUIRE(34 == tokens[17].range.lowerBound);
    REQUIRE(34 == tokens[17].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[17].tokenType);

    REQUIRE("calc.exe" == tokens[18].text);
    REQUIRE(35 == tokens[18].range.lowerBound);
    REQUIRE(42 == tokens[18].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[18].tokenType);
  }

  SECTION("unc") {
    std::string text{R"(\\.\C$\Windows\System32\calc.exe)"};
    auto tokens = Single<FilePath>::instance().tokenize(text);

    REQUIRE(10 == tokens.size());

    REQUIRE(R"(\\)" == tokens[0].text);
    REQUIRE(0 == tokens[0].range.lowerBound);
    REQUIRE(1 == tokens[0].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[0].tokenType);

    REQUIRE("." == tokens[1].text);
    REQUIRE(2 == tokens[1].range.lowerBound);
    REQUIRE(2 == tokens[1].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[1].tokenType);

    REQUIRE("\\" == tokens[2].text);
    REQUIRE(3 == tokens[2].range.lowerBound);
    REQUIRE(3 == tokens[2].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[2].tokenType);

    REQUIRE("C$" == tokens[3].text);
    REQUIRE(4 == tokens[3].range.lowerBound);
    REQUIRE(5 == tokens[3].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::DeviceID) == tokens[3].tokenType);

    REQUIRE("\\" == tokens[4].text);
    REQUIRE(6 == tokens[4].range.lowerBound);
    REQUIRE(6 == tokens[4].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[4].tokenType);

    REQUIRE("Windows" == tokens[5].text);
    REQUIRE(7 == tokens[5].range.lowerBound);
    REQUIRE(13 == tokens[5].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[5].tokenType);

    REQUIRE("\\" == tokens[6].text);
    REQUIRE(14 == tokens[6].range.lowerBound);
    REQUIRE(14 == tokens[6].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[6].tokenType);

    REQUIRE("System32" == tokens[7].text);
    REQUIRE(15 == tokens[7].range.lowerBound);
    REQUIRE(22 == tokens[7].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[7].tokenType);

    REQUIRE("\\" == tokens[8].text);
    REQUIRE(23 == tokens[8].range.lowerBound);
    REQUIRE(23 == tokens[8].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::Separator) == tokens[8].tokenType);

    REQUIRE("calc.exe" == tokens[9].text);
    REQUIRE(24 == tokens[9].range.lowerBound);
    REQUIRE(31 == tokens[9].range.upperBound);
    REQUIRE(TOKEN_TYPE(FilePathTokenType::FSEntryName) == tokens[9].tokenType);
  }
}
