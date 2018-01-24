#ifndef PROTECTION_SAFESTRING_H
#define PROTECTION_SAFESTRING_H

#include <Formatter.h>

#include <string>
#include <iostream>
#include <utility>

namespace protection {
namespace injections {

template <typename LP> class SafeString {
public:
  template <typename... Args>
  static std::pair<std::string, bool> tryFormat(const std::string &formatStr, const Args &... args) {
    return Formatter<LP>::tryFormat(formatStr, args...);
  }

  template <typename... Args> static std::string format(const std::string &format, const Args &... args) {
    auto result = tryFormat(format, args...);
    if (result.second) {
      return result.first;
    }
    throw std::runtime_error{"Attack detected."};
  }
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_SAFESTRING_H
