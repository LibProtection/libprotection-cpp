#ifndef PROJECT_LIBPROTECTIONENGINE_H
#define PROJECT_LIBPROTECTIONENGINE_H

#include "protection/Range.h"
#include "protection/Token.h"

#include <string>
#include <vector>

#define _Out_

enum Grammar { Unknown, Html, Sql, Path, EcmaScript, Url };

namespace protection {
namespace injections {

std::pair<Token, bool> LIBPROTECTION_EXPORT Detect(const std::string &text, const std::vector<Range> &taintedRanges,
                                                   Grammar grammar);
} // namespace injections
} // namespace protection

struct RANGE;
struct TOKEN;
extern "C" bool LIBPROTECTION_EXPORT TryTest(const char *text, RANGE *ranges, int size, _Out_ TOKEN* token);

#endif // PROJECT_LIBPROTECTIONENGINE_H
