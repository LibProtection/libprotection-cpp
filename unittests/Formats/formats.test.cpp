#include <catch/single_include/catch.hpp>
#include <Support/Format.h>

using namespace protection::injections;

template <typename... Args>
static std::string formatTest(const std::string &formatStr, std::vector<Range> &taintedRanges, const Args &... args) {
  typedef fmt::internal::ArgArray<sizeof...(Args)> ArgArray;
  typename ArgArray::Type array{ArgArray::template make<BasicFormatter<char>>(args)...};

  fmt::MemoryWriter w;
  BasicFormatter<char> formatter(fmt::ArgList(fmt::internal::make_type(args...), array), w);
  formatter.format(formatStr);
  taintedRanges = formatter.get_tainted_ranges();
  return w.str();
}

TEST_CASE("NoArgs") {
  std::vector<Range> taintedRanges;
  SECTION("test") {
    REQUIRE("test" == formatTest("test", taintedRanges));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("ArgsInDifferentPositions") {
  std::vector<Range> taintedRanges;
  SECTION("42") {
    REQUIRE("42" == formatTest("{0}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("before 42") {
    REQUIRE("before 42" == formatTest("before {0}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(7 == taintedRanges[0].lowerBound);
    REQUIRE(8 == taintedRanges[0].upperBound);
  }
  SECTION("42 after") {
    REQUIRE("42 after" == formatTest("{0} after", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("before 42 after") {
    REQUIRE("before 42 after" == formatTest("before {0} after", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(7 == taintedRanges[0].lowerBound);
    REQUIRE(8 == taintedRanges[0].upperBound);
  }
  SECTION("answer = 42") {
    REQUIRE("answer = 42" == formatTest("{0} = {1}", taintedRanges, "answer", 42));
    REQUIRE(2 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(5 == taintedRanges[0].upperBound);
    REQUIRE(9 == taintedRanges[1].lowerBound);
    REQUIRE(10 == taintedRanges[1].upperBound);
  }
  SECTION("42 is the answer") {
    REQUIRE("42 is the answer" == formatTest("{1} is the {0}", taintedRanges, "answer", 42));
    REQUIRE(2 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
    REQUIRE(10 == taintedRanges[1].lowerBound);
    REQUIRE(15 == taintedRanges[1].upperBound);
  }
  SECTION("abracadabra") {
    REQUIRE("abracadabra" == formatTest("{0}{1}{0}", taintedRanges, "abra", "cad"));
    REQUIRE(3 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
    REQUIRE(4 == taintedRanges[1].lowerBound);
    REQUIRE(6 == taintedRanges[1].upperBound);
    REQUIRE(7 == taintedRanges[2].lowerBound);
    REQUIRE(10 == taintedRanges[2].upperBound);
  }
  SECTION("42:safe") {
    REQUIRE("42" == formatTest("{0:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("before 42:safe") {
    REQUIRE("before 42" == formatTest("before {0:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("42:safe after") {
    REQUIRE("42 after" == formatTest("{0:safe} after", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("before 42:safe after") {
    REQUIRE("before 42 after" == formatTest("before {0:safe} after", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("answer:safe = 42") {
    REQUIRE("answer = 42" == formatTest("{0:safe} = {1}", taintedRanges, "answer", 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(9 == taintedRanges[0].lowerBound);
    REQUIRE(10 == taintedRanges[0].upperBound);
  }
  SECTION("answer = 42:safe") {
    REQUIRE("answer = 42" == formatTest("{0} = {1:safe}", taintedRanges, "answer", 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(5 == taintedRanges[0].upperBound);
  }
  SECTION("answer:safe = 42:safe") {
    REQUIRE("answer = 42" == formatTest("{0:safe} = {1:safe}", taintedRanges, "answer", 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("42:safe is the answer") {
    REQUIRE("42 is the answer" == formatTest("{1:safe} is the {0}", taintedRanges, "answer", 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(10 == taintedRanges[0].lowerBound);
    REQUIRE(15 == taintedRanges[0].upperBound);
  }
  SECTION("42 is the answer:safe") {
    REQUIRE("42 is the answer" == formatTest("{1} is the {0:safe}", taintedRanges, "answer", 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("42:safe is the answer:safe") {
    REQUIRE("42 is the answer" == formatTest("{1:safe} is the {0:safe}", taintedRanges, "answer", 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("abra{:safe}cadabra") {
    REQUIRE("abracadabra" == formatTest("{0:safe}{1}{0}", taintedRanges, "abra", "cad"));
    REQUIRE(2 == taintedRanges.size());
    REQUIRE(4 == taintedRanges[0].lowerBound);
    REQUIRE(6 == taintedRanges[0].upperBound);
    REQUIRE(7 == taintedRanges[1].lowerBound);
    REQUIRE(10 == taintedRanges[1].upperBound);
  }
  SECTION("abra{:safe}cad{:safe}abra") {
    REQUIRE("abracadabra" == formatTest("{0:safe}{1:safe}{0}", taintedRanges, "abra", "cad"));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(7 == taintedRanges[0].lowerBound);
    REQUIRE(10 == taintedRanges[0].upperBound);
  }
  SECTION("abra{:safe}cad{:safe}abra{:safe}") {
    REQUIRE("abracadabra" == formatTest("{0:safe}{1:safe}{0:safe}", taintedRanges, "abra", "cad"));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("abracad{:safe}abra{:safe}") {
    REQUIRE("abracadabra" == formatTest("{0}{1:safe}{0:safe}", taintedRanges, "abra", "cad"));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("abracadabra{:safe}") {
    REQUIRE("abracadabra" == formatTest("{0}{1}{0:safe}", taintedRanges, "abra", "cad"));
    REQUIRE(2 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
    REQUIRE(4 == taintedRanges[1].lowerBound);
    REQUIRE(6 == taintedRanges[1].upperBound);
  }
  SECTION("abra{:safe}cadabra{:safe}") {
    REQUIRE("abracadabra" == formatTest("{0:safe}{1}{0:safe}", taintedRanges, "abra", "cad"));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(4 == taintedRanges[0].lowerBound);
    REQUIRE(6 == taintedRanges[0].upperBound);
  }
}

TEST_CASE("Escape") {
  std::vector<Range> taintedRanges;
  SECTION("{}") {
    REQUIRE("{}" == formatTest("{{}}", taintedRanges));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{42}") {
    REQUIRE("{42}" == formatTest("{{{0}}}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(1 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
}

TEST_CASE("Format hex") {
  std::vector<Range> taintedRanges;
  SECTION("0") {
    REQUIRE("0" == formatTest("{0:x}", taintedRanges, 0));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(0 == taintedRanges[0].upperBound);
  }
  SECTION("0:safe:x") {
    REQUIRE("0" == formatTest("{0:safe:x}", taintedRanges, 0));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("0:x:safe") {
    REQUIRE("0" == formatTest("{0:x:safe}", taintedRanges, 0));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("42") {
    REQUIRE("42" == formatTest("{0:x}", taintedRanges, 0x42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("42:safe:x") {
    REQUIRE("42" == formatTest("{0:safe:x}", taintedRanges, 0x42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("42:x:safe") {
    REQUIRE("42" == formatTest("{0:x:safe}", taintedRanges, 0x42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("90abcdef") {
    REQUIRE("90abcdef" == formatTest("{0:x}", taintedRanges, 0x90abcdef));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(7 == taintedRanges[0].upperBound);
  }
}

TEST_CASE("AutoArgIndex") {
  std::vector<Range> taintedRanges;
  SECTION("abc") {
    REQUIRE("abc" == formatTest("{}{}{}", taintedRanges, 'a', 'b', 'c'));
    REQUIRE(3 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(0 == taintedRanges[0].upperBound);
    REQUIRE(1 == taintedRanges[1].lowerBound);
    REQUIRE(1 == taintedRanges[1].lowerBound);
    REQUIRE(2 == taintedRanges[2].upperBound);
    REQUIRE(2 == taintedRanges[2].upperBound);
  }
  SECTION("{0}{}") {
    REQUIRE_THROWS_WITH(formatTest("{0}{}", taintedRanges, 'a', 'b'),
                        "cannot switch from manual to automatic argument indexing");
  }
  SECTION("{}{0}") {
    REQUIRE_THROWS_WITH(formatTest("{}{0}", taintedRanges, 'a', 'b'),
                        "cannot switch from automatic to manual argument indexing");
  }
  SECTION("1.2") {
    REQUIRE("1.2" == formatTest("{:.{}}", taintedRanges, 1.2345, 2));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
  SECTION("1.2:safe:.{}") {
    REQUIRE("1.2" == formatTest("{:safe:.{}}", taintedRanges, 1.2345, 2));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("1.2:.{}:safe") {
    REQUIRE("1.2" == formatTest("{:.{}:safe}", taintedRanges, 1.2345, 2));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:safe}:.{}") {
    REQUIRE_THROWS_WITH(formatTest("{0:safe}:.{}", taintedRanges, 1.2345, 2),
                        "cannot switch from manual to automatic argument indexing");
  }
  SECTION("{:.{0}:safe}") {
    REQUIRE_THROWS_WITH(formatTest("{:.{0}:safe}", taintedRanges, 1.2345, 2),
                        "cannot switch from automatic to manual argument indexing");
  }
  SECTION("{}") { REQUIRE_THROWS_WITH(formatTest("{}", taintedRanges), "argument index out of range"); }
}

TEST_CASE("EmptySpecs") {
  std::vector<Range> taintedRanges;
  SECTION("42") {
    REQUIRE("42" == formatTest("{0:}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("42::safe") {
    REQUIRE("42" == formatTest("{0::safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("42:safe:") {
    REQUIRE("42" == formatTest("{0:safe:}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("LeftAlign") {
  std::vector<Range> taintedRanges;
  SECTION("{0:<4}") {
    REQUIRE("42  " == formatTest("{0:<4}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:<4}") {
    REQUIRE("42  " == formatTest("{0:safe:<4}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:<4:safe}") {
    REQUIRE("42  " == formatTest("{0:<4:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:<4o}") {
    REQUIRE("42  " == formatTest("{0:<4o}", taintedRanges, 042));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:<4o}") {
    REQUIRE("42  " == formatTest("{0:safe:<4o}", taintedRanges, 042));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:<4o:safe}") {
    REQUIRE("42  " == formatTest("{0:<4o:safe}", taintedRanges, 042));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("RightAlign") {
  std::vector<Range> taintedRanges;
  SECTION("{0:>4}") {
    REQUIRE("  42" == formatTest("{0:>4}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:>4}") {
    REQUIRE("  42" == formatTest("{0:safe:>4}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:>4:safe}") {
    REQUIRE("  42" == formatTest("{0:>4:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("NumericAlign") {
  std::vector<Range> taintedRanges;
  SECTION("{0:=4}") {
    REQUIRE("  42" == formatTest("{0:=4}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:=4}") {
    REQUIRE("  42" == formatTest("{0:safe:=4}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:=4:safe}") {
    REQUIRE("  42" == formatTest("{0:=4:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:=+4}") {
    REQUIRE("+ 42" == formatTest("{0:=+4}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:=+4}") {
    REQUIRE("+ 42" == formatTest("{0:safe:=+4}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:=+4:safe}") {
    REQUIRE("+ 42" == formatTest("{0:=+4:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("CenterAlign") {
  std::vector<Range> taintedRanges;
  SECTION("{0:^5}") {
    REQUIRE(" 42  " == formatTest("{0:^5}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(4 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:^5}") {
    REQUIRE(" 42  " == formatTest("{0:safe:^5}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:^6}") {
    REQUIRE(" -42  " == formatTest("{0:^6}", taintedRanges, -42.0));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(5 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:^6}") {
    REQUIRE(" -42  " == formatTest("{0:safe:^6}", taintedRanges, -42.0));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:^6:safe}") {
    REQUIRE(" -42  " == formatTest("{0:^6:safe}", taintedRanges, -42.0));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:^8}") {
    REQUIRE(" 0xface " == formatTest("{0:^8}", taintedRanges, reinterpret_cast<void *>(0xface)));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(7 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:^8}") {
    REQUIRE(" 0xface " == formatTest("{0:safe:^8}", taintedRanges, reinterpret_cast<void *>(0xface)));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:^8:safe}") {
    REQUIRE(" 0xface " == formatTest("{0:^8:safe}", taintedRanges, reinterpret_cast<void *>(0xface)));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("Fill") {
  std::vector<Range> taintedRanges;
  SECTION("{0:{<5}") { REQUIRE_THROWS_WITH(formatTest("{0:{<5}", taintedRanges, 'c'), "invalid fill character '{'"); }
  SECTION("{0:safe:{<5}") {
    REQUIRE_THROWS_WITH(formatTest("{0:safe:{<5}", taintedRanges, 'c'), "invalid fill character '{'");
  }
  SECTION("{0:{<5:safe}") {
    REQUIRE_THROWS_WITH(formatTest("{0:{<5:safe}", taintedRanges, 'c'), "invalid fill character '{'");
  }
  SECTION("{0:{<5}}") { REQUIRE_THROWS_WITH(formatTest("{0:{<5}}", taintedRanges, 'c'), "invalid fill character '{'"); }
  SECTION("{0:*>4}") {
    REQUIRE("**42" == formatTest("{0:*>4}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:*>4}") {
    REQUIRE("**42" == formatTest("{0:safe:*>4}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:*>4:safe}") {
    REQUIRE("**42" == formatTest("{0:*>4:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:*>8}") {
    REQUIRE("**0xface" == formatTest("{0:*>8}", taintedRanges, reinterpret_cast<void *>(0xface)));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(7 == taintedRanges[0].upperBound);
  }
}

TEST_CASE("PlusSign") {
  std::vector<Range> taintedRanges;
  SECTION("{0:+}") {
    REQUIRE("+42" == formatTest("{0:+}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:+}") {
    REQUIRE("+42" == formatTest("{0:safe:+}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:+:safe}") {
    REQUIRE("+42" == formatTest("{0:+:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
}

TEST_CASE("SpaceSign") {
  std::vector<Range> taintedRanges;
  SECTION("{0: }") {
    REQUIRE(" 42" == formatTest("{0: }", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe: }") {
    REQUIRE(" 42" == formatTest("{0:safe: }", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0: :safe}") {
    REQUIRE(" 42" == formatTest("{0: :safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0: }-") {
    REQUIRE("-42" == formatTest("{0: }", taintedRanges, -42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe: }-") {
    REQUIRE("-42" == formatTest("{0:safe: }", taintedRanges, -42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0: :safe}-") {
    REQUIRE("-42" == formatTest("{0: :safe}", taintedRanges, -42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0: }u") {
    REQUIRE_THROWS_WITH(formatTest("{0: }", taintedRanges, 42u), "format specifier ' ' requires signed argument");
  }
  SECTION("{0:safe: }u") {
    REQUIRE_THROWS_WITH(formatTest("{0:safe: }", taintedRanges, 42u), "format specifier ' ' requires signed argument");
  }
  SECTION("{0: :safe}u") {
    REQUIRE_THROWS_WITH(formatTest("{0: :safe}", taintedRanges, 42u), "format specifier ' ' requires signed argument");
  }
}

TEST_CASE("HashFlag") {
  std::vector<Range> taintedRanges;
  SECTION("{0:#}") {
    REQUIRE("42" == formatTest("{0:#}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(1 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:#}") {
    REQUIRE("42" == formatTest("{0:safe:#}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#:safe}") {
    REQUIRE("42" == formatTest("{0:safe:#}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#}-") {
    REQUIRE("-42" == formatTest("{0:#}", taintedRanges, -42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(2 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:#}-") {
    REQUIRE("-42" == formatTest("{0:safe:#}", taintedRanges, -42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#:safe}-") {
    REQUIRE("-42" == formatTest("{0:#:safe}", taintedRanges, -42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#b}") {
    REQUIRE("0b101010" == formatTest("{0:#b}", taintedRanges, 42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(7 == taintedRanges[0].upperBound);
  }
  SECTION("{0:safe:#b}") {
    REQUIRE("0b101010" == formatTest("{0:safe:#b}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#b:safe}") {
    REQUIRE("0b101010" == formatTest("{0:#b:safe}", taintedRanges, 42));
    REQUIRE(0 == taintedRanges.size());
  }
  SECTION("{0:#b}-") {
    REQUIRE("-0b101010" == formatTest("{0:#b}", taintedRanges, -42));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(8 == taintedRanges[0].upperBound);
  }
  SECTION("{0:#o}") {
    REQUIRE("-042" == formatTest("{0:#o}", taintedRanges, -042));
    REQUIRE(1 == taintedRanges.size());
    REQUIRE(0 == taintedRanges[0].lowerBound);
    REQUIRE(3 == taintedRanges[0].upperBound);
  }
}

TEST_CASE("Duplicate") {
  std::vector<Range> taintedRanges;
  SECTION("{0:safe:safe}") {
    REQUIRE_THROWS_WITH(formatTest("{0:safe:safe}", taintedRanges, 42), "missing '}' in format string");
  }

  SECTION("{:safe0:safe}") {
    REQUIRE_THROWS_WITH(formatTest("{:safe0:safe}", taintedRanges, 42), "missing '}' in format string");
  }

  SECTION("{:safe:safe0}") {
    REQUIRE_THROWS_WITH(formatTest("{:safe:safe0}", taintedRanges, 42), "missing '}' in format string");
  }
  SECTION("{0:safe:x:safe}") {
    REQUIRE_THROWS_WITH(formatTest("{0:safe:x:safe}", taintedRanges, 42), "missing '}' in format string");
  }
}
