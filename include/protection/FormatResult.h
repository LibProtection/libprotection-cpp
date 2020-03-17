#ifndef LIBPROTECTION_FORMATRESULT_H
#define LIBPROTECTION_FORMATRESULT_H

#include "protection/Token.h"

#include <string>
#include <vector>

namespace protection {
namespace injections {

struct LIBPROTECTION_EXPORT FormatResult {
  FormatResult(std::vector<Token> t, bool detected, size_t index, const std::string &s);
  FormatResult() = default;

  static FormatResult success(std::vector<Token> tokens, const std::string &formattedString);

  static FormatResult fail(std::vector<Token> tokens, int injectionPointIndex);

  // array of tokenization artifacts
  std::vector<Token> tokens;
  // true if an attack was detected during formatting
  bool isAttackDetected{false};
  // if  isAttackDetected == true, contains an index of attacked args element
  size_t injectionPointIndex;
  // result of Formatting (should be empty if isAttackDetected == true)
  std::string formattedString;
};
} // namespace injections
} // namespace protection

#endif // LIBPROTECTION_FORMATRESULT_H
