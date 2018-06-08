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
