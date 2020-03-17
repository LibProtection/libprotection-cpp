#include "protection/LibProtectionEngine.h"
#include "protection/LanguageService.h"
#include "protection/Languages.h"

namespace protection {
namespace injections {

std::pair<Token, bool> Detect(const std::string &text, const std::vector<Range> &taintedRanges, Grammar grammar) {
  switch (grammar) {
  case Grammar::Html:
    return LanguageService::validate<Html>(text, taintedRanges);

  case Grammar::Sql:
    return LanguageService::validate<Sql>(text, taintedRanges);

  case Grammar::Path:
    return LanguageService::validate<FilePath>(text, taintedRanges);

  case Grammar::EcmaScript:
    return LanguageService::validate<JavaScript>(text, taintedRanges);

  case Grammar::Url:
    return LanguageService::validate<Url>(text, taintedRanges);

  default:
    throw std::runtime_error{"Unsupported language provider."};
  }
}

} // namespace injections
} // namespace protection

struct RANGE {
  int x;
  int y;
};

struct TOKEN {
  bool isTrivial;
  unsigned tokenType;

  //const char *text;
  //size_t text_size;
  //int lowerBound;
  //int upperBound;
  RANGE range;
  char *text;
  int textLen;
};

bool TryTest(const char *text, RANGE *ranges, int size, _Out_ TOKEN *token) {
  std::cout << "in test" << std::endl;
  std::cout << text << std::endl;
  auto str_text = std::string(text);

  auto rvect = std::vector<Range>{};
  for (int i = 0; i < size; ++i) {
    rvect.emplace_back(ranges->x, ranges->y);
    ranges++;
  } //(ranges, ranges + size);
  auto result = protection::injections::LanguageService::validate<protection::injections::Html>(str_text, rvect);
  if (!result.second) {
    token->tokenType = result.first.tokenType;
    token->isTrivial = result.first.isTrivial;
    //token->text = result.first.text.c_str();
    //token->text_size = result.first.text.size();

    token->range.x  = result.first.range.lowerBound;
    token->range.y	= result.first.range.upperBound;
    
	auto sz = result.first.text.size();
	auto txt = malloc(sz);
    strncpy((char*)txt, "blah"/*result.first.text.c_str()*/, 4);
    token->text = (char *)txt;
    token->textLen = 4;
  }

  return result.second;
}