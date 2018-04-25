#include "protection/LibProtectionEngine.h"
#include "protection/LanguageService.h"
#include "protection/Languages.h"

namespace protection {
namespace injections {

bool Detect(const std::string &text, const std::vector<Range> &taintedRanges, Grammar grammar) {
  switch (grammar) {
  case Grammar::Html:
    return LanguageService::Validate<Html>(text, taintedRanges).second;

  case Grammar::Sql:
    return LanguageService::Validate<Sql>(text, taintedRanges).second;

  case Grammar::Path:
    return LanguageService::Validate<FilePath>(text, taintedRanges).second;

  case Grammar::EcmaScript:
    return LanguageService::Validate<JavaScript>(text, taintedRanges).second;

  case Grammar::Url:
    return LanguageService::Validate<Url>(text, taintedRanges).second;

  default:
    throw std::runtime_error{"Unsupported language provider."};
  }
}

} // namespace injections
} // namespace protection
