#ifndef PROTECTION_SAFESTRING_H
#define PROTECTION_SAFESTRING_H

#include "protection/FormatResult.h"
#include "protection/Formatter.h"

#include <algorithm>
#include <cassert>
//#include <iostream>
#include <string>
#include <utility>

namespace protection {
namespace injections {

template <typename LP> class SafeString {
public:
  template <typename... Args>
  static std::pair<std::string, bool> tryFormat(const std::string &formatStr, const Args &... args) {
    auto result = formatEx(formatStr, args...);
    return (result.isAttackDetected) ? std::make_pair(std::string{}, false)
                                     : std::make_pair(result.formattedString, true);
  }

  template <typename... Args> static std::string format(const std::string &formatStr, const Args &... args) {
    auto result = tryFormat(formatStr, args...);
    if (result.second) {
      return result.first;
    }
    throw std::runtime_error{"Attack detected."};
  }

private:
  template <typename... Args> static FormatResult formatEx(const std::string &formatStr, const Args &... args) {

    std::string formattedString;
    std::vector<Range> taintedRanges;
    std::vector<size_t> associatedToRangeIndexes;
    std::tie(formattedString, taintedRanges, associatedToRangeIndexes) = Formatter<LP>::format(formatStr, args...);

    auto sanitizeResult = LanguageService::trySanitize<LP>(formattedString, taintedRanges);
    if (sanitizeResult.success) {
      return FormatResult::success(sanitizeResult.tokens, sanitizeResult.sanitizedString);
    }

    auto it = std::find_if(taintedRanges.begin(), taintedRanges.end(), [&](const Range &range) -> bool {
      return range.overlaps(sanitizeResult.attackToken.range);
    });
    if (it == taintedRanges.end()) {
      throw std::runtime_error{"Cannot find attack argument for attack token."};
    }
    auto attackArgumentIndex = std::distance(taintedRanges.begin(), it);
    return FormatResult::fail(sanitizeResult.tokens, associatedToRangeIndexes[attackArgumentIndex]);
  }
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_SAFESTRING_H
