#ifndef PROJECT_LIBPROTECTIONENGINE_H
#define PROJECT_LIBPROTECTIONENGINE_H

#include "protection/Range.h"
#include "protection/Token.h"

#include <string>
#include <vector>

enum Grammar { Unknown, Html, Sql, Path, EcmaScript, Url };

namespace protection {
namespace injections {

std::pair<Token, bool> Detect(const std::string &text, const std::vector<Range> &taintedRanges, Grammar grammar);
} // namespace injections
} // namespace protection

#endif // PROJECT_LIBPROTECTIONENGINE_H
