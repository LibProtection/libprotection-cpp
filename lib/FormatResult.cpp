#include "protection/FormatResult.h"

namespace protection {
namespace injections {

FormatResult::FormatResult(std::vector<Token> t, bool detected, size_t index, const std::string &s)
    : tokens{std::move(t)}, isAttackDetected{detected}, injectionPointIndex{index}, formattedString{s} {}

FormatResult FormatResult::success(std::vector<Token> tokens, const std::string &formattedString) {
  return FormatResult(tokens, false, -1, formattedString);
}

FormatResult FormatResult::fail(std::vector<Token> tokens, int injectionPointIndex) {
  return FormatResult(tokens, true, injectionPointIndex, {});
}

} // namespace injections
} // namespace protection
