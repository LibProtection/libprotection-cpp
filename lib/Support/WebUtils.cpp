#include "Support/WebUtils.h"
#include <algorithm>
#include <sstream>

namespace utils {

bool isUrlSafeChar(char ch) {
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
    return true;

  switch (ch) {
  case '-':
  case '_':
  case '.':
  case '!':
  case '*':
  case '(':
  case ')':
    return true;
  default:
    break;
  }

  return false;
}

std::string encode(const char ch) {
  std::stringstream ss;
  ss << std::hex << (ch >> 4);
  ss << std::hex << (ch & 0x0F);
  auto res = ss.str();
  std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return std::toupper(c); });
  return res;
}

std::string UrlEncode(const std::string &s) {
  if (s.empty()) {
    return s;
  }

  std::stringstream ss;
  for (size_t i = 0; i < s.length(); ++i) {
    if (isUrlSafeChar(s[i])) {
      ss << s[i];
    } else if (s[i] == ' ') {
      ss << '+';
    } else {
      ss << '%';
      ss << encode(s[i]);
    }
  }

  return ss.str();
}
} // namespace utils
