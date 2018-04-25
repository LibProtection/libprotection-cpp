#ifndef PROTECTION_FORMATTER_H
#define PROTECTION_FORMATTER_H

#include "protection/Single.h"
#include "protection/LanguageService.h"
#include "support/Format.h"

#include <string>
#include <tuple>

namespace protection {
namespace injections {

template <typename LP> class Formatter {
public:
  template <typename... Args>
  static std::tuple<std::string, std::vector<Range>, std::vector<size_t>> format(const std::string &formatStr,
                                                                                 const Args &... args) {

    typedef fmt::internal::ArgArray<sizeof...(Args)> ArgArray;
    typename ArgArray::Type array{ArgArray::template make<BasicFormatter<char>>(args)...};

    fmt::MemoryWriter w;
    BasicFormatter<char> formatter(fmt::ArgList(fmt::internal::make_type(args...), array), w);
    formatter.format(formatStr);

    return std::make_tuple(w.c_str(), formatter.get_tainted_ranges(), formatter.get_associated_to_range_indexes());
  }
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_FORMATTER_H
