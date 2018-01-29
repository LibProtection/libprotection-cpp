#ifndef PROTECTION_FORMATTER_H
#define PROTECTION_FORMATTER_H

#include <Single.h>
#include <LanguageService.h>
#include <Support/Format.h>

#include <string>

namespace protection {
namespace injections {

template <typename LP> class Formatter {
public:
  template <typename... Args>
  static std::pair<std::string, bool> tryFormat(const std::string &formatStr, const Args &... args) {

    typedef fmt::internal::ArgArray<sizeof...(Args)> ArgArray;
    typename ArgArray::Type array{ArgArray::template make<BasicFormatter<char>>(args)...};

    fmt::MemoryWriter w;
    BasicFormatter<char> formatter(fmt::ArgList(fmt::internal::make_type(args...), array), w);
    formatter.format(formatStr);

    return LanguageService::trySanitize<LP>(w.str(), formatter.get_tainted_ranges());
  }
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_FORMATTER_H
