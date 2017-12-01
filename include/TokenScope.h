#ifndef PROTECTION_TOKENSCOPE_H
#define PROTECTION_TOKENSCOPE_H

#include "Range.h"
#include "Token.h"

#include <algorithm>
#include <vector>

namespace protection {
struct TokenScope {
  Range range;
  std::vector<Token> tokens;
  bool isTrivial() {
    return std::all_of(tokens.begin(), tokens.end(), [](const Token &t) { return t.isTrivial; });
  }

  explicit TokenScope(Range r) : range{r} {}
};

} // namespace protection
#endif // PROTECTION_TOKENSCOPE_H
