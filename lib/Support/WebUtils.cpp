#include "support/WebUtils.h"
#include <algorithm>
#include <cassert>
#include <sstream>
#include <locale>
#include <codecvt>
#include <map>
#include <iostream>
#include <iomanip>
#include <cctype>

#define HIGH_SURROGATE_START 0xD800
#define HIGH_SURROGATE_END 0xDBFF
#define LOW_SURROGATE_START 0xDC00
#define LOW_SURROGATE_END 0xDFFF
#define UNICODE_PLANE00_END 0x00FFFF
#define UNICODE_PLANE01_START 0x10000
#define UNICODE_PLANE16_END 0x10FFFF
#define UNICODE_REPLACEMENT_CHAR 0xFFFD

namespace utility {

bool isSurrogate(uint16_t c) { return c >= HIGH_SURROGATE_START && c <= LOW_SURROGATE_END; }

int indexOfHtmlAttributeEncodingChars(const std::string &s, size_t startPos) {
  assert(0 <= startPos && startPos <= s.length() && "0 <= startPos && startPos <= s.Length");
  auto cch = s.length() - startPos;
  for (size_t i = startPos; cch > 0; ++i, --cch) {
    if (s[i] <= '<') {
      switch (s[i]) {
      case '<':
      case '"':
      case '\'':
      case '&':
        return static_cast<int>(s.length() - cch);
      default:
        continue;
      }
    }
  }

  return -1;
}

std::string HtmlAttributeEncode(const std::string &s) {
  if (s.empty()) {
    return {};
  }
  auto index = indexOfHtmlAttributeEncodingChars(s, 0);
  /// Return string if there is nothing to encode
  if (index == -1) {
    return s;
  }

  auto cch = s.length() - index;
  std::stringstream ss;
  size_t i{0};
  while (index-- > 0) {
    ss << s[i++];
  }

  while (cch-- > 0) {
    if (s[i] <= '<') {
      switch (s[i]) {
      case '<':
        ss << "&lt;";
        break;
      case '"':
        ss << "&quot;";
        break;
      case '\'':
        ss << "&#39;";
        break;
      case '&':
        ss << "&amp;";
        break;
      default:
        ss << s[i];
        break;
      }
    } else {
      ss << s[i];
    }
    ++i;
  }

  return ss.str();
}

std::string HtmlEncode(const std::string &s, size_t startPos) {
  std::stringstream ss;

  auto len = s.length();
  decltype(len) i{0};
  while (i < startPos) {
    ss << s[i++];
  }
  while (i < len) {
    if (((s[i] & 0x80) == 0) && (s[i] <= '>')) {
      switch (s[i]) {
      case '<':
        ss << "&lt;";
        break;
      case '>':
        ss << "&gt;";
        break;
      case '"':
        ss << "&quot;";
        break;
      case '\'':
        ss << "&#39;";
        break;
      case '&':
        ss << "&amp;";
        break;
      default:
        ss << s[i];
        break;
      }
    } else {

      if ((s[i] & 0xE0) == 0xC0) {
        // wrong character sequence
        if ((i + 1) >= len || ((s[i + 1] & 0xC0) != 0x80)) {
          ss << UNICODE_REPLACEMENT_CHAR;
        } else {

          auto val = ((s[i] & 0x1F) << 6) | ((s[i + 1] & 0x3F));

          if (val >= 160 && val < 256) {
            // s needs to be encoded
            ss << "&#";
            ss << std::to_string(val).c_str();
            ss << ';';
          } else {
            ss << s[i] << s[i + 1];
          }
        }
        i += 1;

      } else if ((s[i] & 0xF0) == 0xE0) {
        // wrong character sequence
        if (((i + 2) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80)) {
          ss << UNICODE_REPLACEMENT_CHAR;
        } else {

          auto val = static_cast<uint16_t>(((s[i] & 0xF) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F));

          if (isSurrogate(val)) {
            ss << UNICODE_REPLACEMENT_CHAR;
          } else {
            ss << s[i] << s[i + 1] << s[i + 2];
          }
        }

        i += 2;

      } else if ((s[i] & 0xF8) == 0xF0) {
        // wrong character sequence
        if (((i + 3) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80) ||
            ((s[i + 3] & 0xC0) != 0x80)) {
          ss << UNICODE_REPLACEMENT_CHAR;
        } else {

          auto val = ((s[i] & 0x7) << 18) | ((s[i + 1] & 0x3F) << 12) | ((s[i + 2] & 0x3F) << 6) | (s[i + 3] & 0x3F);
          ss << "&#";
          ss << std::to_string(val).c_str();
          ss << ';';
        }
        i += 3;
      } else if ((s[i] & 0xC0) == 0x80) {
        // Separate 10xx xxxx byte processing. Treated as wrong character.
        ss << UNICODE_REPLACEMENT_CHAR;
      } else {
        // write out the character directly
        ss << s[i];
      }
    }

    ++i;
  }

  return ss.str();
}

/// Return -1 if no encoding required or the nonnegative number (character
/// index) if the input string needs to be encoded.
/// Encoding will be provided if:
/// - special character met;
/// - surrogate character met;
/// - the bit mask of the utf8 code point does not satisfy the requirements;
int64_t indexOfHtmlEncodingChars(const std::string &s, size_t startPos) {
  assert(0 <= startPos && startPos <= s.length() && "0 <= startPos && startPos <= s.length()");

  auto len = s.length();
  for (auto i = startPos; i < len; ++i) {
    if (((s[i] & 0x80) == 0) && (s[i] <= '>')) {
      switch (s[i]) {
      case '<':
      case '>':
      case '"':
      case '\'':
      case '&':
        return i;
      default:
        continue;
      }
    } else if ((s[i] & 0xE0) == 0xC0) {
      if (((i + 1) >= len) || ((s[i + 1] & 0xC0) != 0x80)) {
        return i;
      }

      auto val = ((s[i] & 0x1F) << 6) | ((s[i + 1] & 0x3F));
      if (val >= 160 && val < 256) {
        return i;
      }
      i += 1;
    } else if ((s[i] & 0xF0) == 0xE0) {
      if (((i + 2) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80)) {
        return i;
      }

      auto val = static_cast<uint16_t>(((s[i] & 0xF) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F));
      if (isSurrogate(val)) {
        return i;
      }
      i += 2;

    } else if ((s[i] & 0xF8) == 0xF0) {
      if (((i + 3) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80) ||
          ((s[i + 3] & 0xC0) != 0x80)) {
        return i;
      }

      auto val =
          (uint32_t)((s[i] & 0x7) << 18) | ((s[i + 1] & 0x3F) << 12) | ((s[i + 2] & 0x3F) << 6) | (s[i + 3] & 0x3F);

      if (val >= UNICODE_PLANE01_START) {
        return i;
      }

      i += 3;
    }
  }

  return -1;
}

std::string HtmlEncode(const std::string &s) {
  if (s.empty()) {
    return s;
  }

  // return input string if we don't have anything to encode
  auto index = indexOfHtmlEncodingChars(s, 0);
  if (index == -1) {
    return s;
  }

  return HtmlEncode(s, static_cast<size_t>(index));
}

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
    return false;
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
      ss << encode(s[i]).c_str();
    }
  }

  return ss.str();
}

std::string JavaScriptStringEncode(const std::string &s, bool addDoubleQuotes) {
  if (s.empty()) {
    return addDoubleQuotes ? "\"\"" : std::string{};
  }

  auto len = s.length();
  bool needEncode = false;
  char c;
  for (size_t i = 0; i < len; ++i) {
    c = s[i];

    if ((c >= 0 && c <= 31) || c == 34 || c == 39 || c == 60 || c == 62 || c == 92) {
      needEncode = true;
      break;
    }
  }

  if (!needEncode)
    return addDoubleQuotes ? "\"" + s + "\"" : s;

  std::stringstream ss;
  if (addDoubleQuotes)
    ss << '"';

  for (size_t i = 0; i < len; i++) {
    c = s[i];
    if ((c >= 0 && c <= 7) || c == 11 || (c >= 14 && c <= 31) || c == 39 || c == 60 || c == 62) {
      std::ios oldState(nullptr);
      oldState.copyfmt(ss);
      ss << "\\u" << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(c);
      ss.copyfmt(oldState);
    } else {
      switch (c) {
      case '\b':
        ss << "\\b";
        break;

      case '\t':
        ss << "\\t";
        break;

      case '\n':
        ss << "\\n";
        break;

      case '\f':
        ss << "\\f";
        break;

      case '\r':
        ss << "\\r";
        break;

      case '\"':
        ss << "\\\"";
        break;

      case '\\':
        ss << "\\\\";
        break;

      default:
        ss << c;
        break;
      }
    }
  }

  if (addDoubleQuotes)
    ss << '"';

  return ss.str();
}

std::string JavaScriptStringEncode(const std::string &s) { return JavaScriptStringEncode(s, false); }
} // namespace utility
