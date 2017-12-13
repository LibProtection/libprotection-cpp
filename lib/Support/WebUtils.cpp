#include "Support/WebUtils.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <locale>
#include <codecvt>
#include <map>
#include <regex>
#include <iostream>

#define HIGH_SURROGATE_START 0xD800
#define HIGH_SURROGATE_END 0xDBFF
#define LOW_SURROGATE_START 0xDC00
#define LOW_SURROGATE_END 0xDFFF
#define UNICODE_PLANE00_END 0x00FFFF
#define UNICODE_PLANE01_START 0x10000
#define UNICODE_PLANE16_END 0x10FFFF
#define UNICODE_REPLACEMENT_CHAR 0xFFFD

namespace utils {

bool isSurrogate(uint16_t c) { return c >= HIGH_SURROGATE_START && c <= LOW_SURROGATE_END; }

std::string HtmlAttributeEncode(const std::string &s) { throw std::runtime_error{"Not implemented yet!"}; }

std::string HtmlEncode(const std::string &s, int startPos) {
  std::stringstream ss;

  auto len = s.length();
  auto i{0};
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
            ss << std::to_string(val);
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

          auto val = ((s[i] & 0xF) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F);

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
          ss << std::to_string(val);
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
int indexOfHtmlEncodingChars(const std::string &s, int startPos) {
  assert(0 <= startPos && startPos <= static_cast<int>(s.length()) && "0 <= startPos && startPos <= s.length()");

  auto len = s.length();
  for (size_t i = startPos; i < len; ++i) {
    if (((s[i] & 0x80) == 0) && (s[i] <= '>')) {
      switch (s[i]) {
      case '<':
      case '>':
      case '"':
      case '\'':
      case '&':
        return i;
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

      auto val = ((s[i] & 0xF) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F);
      if (isSurrogate(val)) {
        return i;
      }
      i += 2;

    } else if ((s[i] & 0xF8) == 0xF0) {
      if (((i + 3) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80) ||
          ((s[i + 3] & 0xC0) != 0x80)) {
        return i;
      }

      uint32_t val = ((s[i] & 0x7) << 18) | ((s[i + 1] & 0x3F) << 12) | ((s[i + 2] & 0x3F) << 6) | (s[i + 3] & 0x3F);

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
  int index = indexOfHtmlEncodingChars(s, 0);
  if (index == -1) {
    return s;
  }

  return HtmlEncode(s, index);
}

bool isSurrogatePair(uint16_t highSurrogate, uint16_t lowSurrogate) {
  return highSurrogate >= HIGH_SURROGATE_START && highSurrogate <= HIGH_SURROGATE_END &&
         lowSurrogate >= LOW_SURROGATE_START && lowSurrogate <= LOW_SURROGATE_END;
}

bool stringRequiresHtmlDecoding(const std::string &s) {

  auto len = s.length();
  for (size_t i = 0; i < len; ++i) {
    if (s[i] == '&') {
      return true;
    } else if ((s[i] & 0xF0) == 0xE0) {
      if (((i + 2) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80)) {
        throw std::runtime_error{"Invalid character sequence."};
      }

      auto val = ((s[i] & 0xF) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F);

      if (isSurrogate(val)) {
        return true;
      }
      i += 2;
    } else if ((s[i] & 0xF8) == 0xF0) {

      if (((i + 3) >= len) || ((s[i + 1] & 0xC0) != 0x80) || ((s[i + 2] & 0xC0) != 0x80) ||
          ((s[i + 3] & 0xC0) != 0x80)) {
        throw std::runtime_error{"Invalid character sequence."};
      }

      auto val = ((s[i] & 0x7) << 18) | ((s[i + 1] & 0x3F) << 12) | ((s[i + 2] & 0x3F) << 6) | (s[i + 3] & 0x3F);

      if (val >= UNICODE_PLANE01_START) {
        return true;
      }
      i += 3;
    }
  }
  return false;
}

std::string htmlEntitiesLookup(const std::string &entity) {
  static std::map<std::string, std::string> entities{
      {"quot", "\u0022"},   {"amp", "\u0026"},    {"apos", "\u0027"},    {"lt", "\u003c"},       {"gt", "\u003e"},
      {"nbsp", "\u00a0"},   {"iexcl", "\u00a1"},  {"cent", "\u00a2"},    {"pound", "\u00a3"},    {"curren", "\u00a4"},
      {"yen", "\u00a5"},    {"brvbar", "\u00a6"}, {"sect", "\u00a7"},    {"uml", "\u00a8"},      {"copy", "\u00a9"},
      {"ordf", "\u00aa"},   {"laquo", "\u00ab"},  {"not", "\u00ac"},     {"shy", "\u00ad"},      {"reg", "\u00ae"},
      {"macr", "\u00af"},   {"deg", "\u00b0"},    {"plusmn", "\u00b1"},  {"sup2", "\u00b2"},     {"sup3", "\u00b3"},
      {"acute", "\u00b4"},  {"micro", "\u00b5"},  {"para", "\u00b6"},    {"middot", "\u00b7"},   {"cedil", "\u00b8"},
      {"sup1", "\u00b9"},   {"ordm", "\u00ba"},   {"raquo", "\u00bb"},   {"frac14", "\u00bc"},   {"frac12", "\u00bd"},
      {"frac34", "\u00be"}, {"iquest", "\u00bf"}, {"Agrave", "\u00c0"},  {"Aacute", "\u00c1"},   {"Acirc", "\u00c2"},
      {"Atilde", "\u00c3"}, {"Auml", "\u00c4"},   {"Aring", "\u00c5"},   {"AElig", "\u00c6"},    {"Ccedil", "\u00c7"},
      {"Egrave", "\u00c8"}, {"Eacute", "\u00c9"}, {"Ecirc", "\u00ca"},   {"Euml", "\u00cb"},     {"Igrave", "\u00cc"},
      {"Iacute", "\u00cd"}, {"Icirc", "\u00ce"},  {"Iuml", "\u00cf"},    {"ETH", "\u00d0"},      {"Ntilde", "\u00d1"},
      {"Ograve", "\u00d2"}, {"Oacute", "\u00d3"}, {"Ocirc", "\u00d4"},   {"Otilde", "\u00d5"},   {"Ouml", "\u00d6"},
      {"times", "\u00d7"},  {"Oslash", "\u00d8"}, {"Ugrave", "\u00d9"},  {"Uacute", "\u00da"},   {"Ucirc", "\u00db"},
      {"Uuml", "\u00dc"},   {"Yacute", "\u00dd"}, {"THORN", "\u00de"},   {"szlig", "\u00df"},    {"agrave", "\u00e0"},
      {"aacute", "\u00e1"}, {"acirc", "\u00e2"},  {"atilde", "\u00e3"},  {"auml", "\u00e4"},     {"aring", "\u00e5"},
      {"aelig", "\u00e6"},  {"ccedil", "\u00e7"}, {"egrave", "\u00e8"},  {"eacute", "\u00e9"},   {"ecirc", "\u00ea"},
      {"euml", "\u00eb"},   {"igrave", "\u00ec"}, {"iacute", "\u00ed"},  {"icirc", "\u00ee"},    {"iuml", "\u00ef"},
      {"eth", "\u00f0"},    {"ntilde", "\u00f1"}, {"ograve", "\u00f2"},  {"oacute", "\u00f3"},   {"ocirc", "\u00f4"},
      {"otilde", "\u00f5"}, {"ouml", "\u00f6"},   {"divide", "\u00f7"},  {"oslash", "\u00f8"},   {"ugrave", "\u00f9"},
      {"uacute", "\u00fa"}, {"ucirc", "\u00fb"},  {"uuml", "\u00fc"},    {"yacute", "\u00fd"},   {"thorn", "\u00fe"},
      {"yuml", "\u00ff"},   {"OElig", "\u0152"},  {"oelig", "\u0153"},   {"Scaron", "\u0160"},   {"scaron", "\u0161"},
      {"Yuml", "\u0178"},   {"fnof", "\u0192"},   {"circ", "\u02c6"},    {"tilde", "\u02dc"},    {"Alpha", "\u0391"},
      {"Beta", "\u0392"},   {"Gamma", "\u0393"},  {"Delta", "\u0394"},   {"Epsilon", "\u0395"},  {"Zeta", "\u0396"},
      {"Eta", "\u0397"},    {"Theta", "\u0398"},  {"Iota", "\u0399"},    {"Kappa", "\u039a"},    {"Lambda", "\u039b"},
      {"Mu", "\u039c"},     {"Nu", "\u039d"},     {"Xi", "\u039e"},      {"Omicron", "\u039f"},  {"Pi", "\u03a0"},
      {"Rho", "\u03a1"},    {"Sigma", "\u03a3"},  {"Tau", "\u03a4"},     {"Upsilon", "\u03a5"},  {"Phi", "\u03a6"},
      {"Chi", "\u03a7"},    {"Psi", "\u03a8"},    {"Omega", "\u03a9"},   {"alpha", "\u03b1"},    {"beta", "\u03b2"},
      {"gamma", "\u03b3"},  {"delta", "\u03b4"},  {"epsilon", "\u03b5"}, {"zeta", "\u03b6"},     {"eta", "\u03b7"},
      {"theta", "\u03b8"},  {"iota", "\u03b9"},   {"kappa", "\u03ba"},   {"lambda", "\u03bb"},   {"mu", "\u03bc"},
      {"nu", "\u03bd"},     {"xi", "\u03be"},     {"omicron", "\u03bf"}, {"pi", "\u03c0"},       {"rho", "\u03c1"},
      {"sigmaf", "\u03c2"}, {"sigma", "\u03c3"},  {"tau", "\u03c4"},     {"upsilon", "\u03c5"},  {"phi", "\u03c6"},
      {"chi", "\u03c7"},    {"psi", "\u03c8"},    {"omega", "\u03c9"},   {"thetasym", "\u03d1"}, {"upsih", "\u03d2"},
      {"piv", "\u03d6"},    {"ensp", "\u2002"},   {"emsp", "\u2003"},    {"thinsp", "\u2009"},   {"zwnj", "\u200c"},
      {"zwj", "\u200d"},    {"lrm", "\u200e"},    {"rlm", "\u200f"},     {"ndash", "\u2013"},    {"mdash", "\u2014"},
      {"lsquo", "\u2018"},  {"rsquo", "\u2019"},  {"sbquo", "\u201a"},   {"ldquo", "\u201c"},    {"rdquo", "\u201d"},
      {"bdquo", "\u201e"},  {"dagger", "\u2020"}, {"Dagger", "\u2021"},  {"bull", "\u2022"},     {"hellip", "\u2026"},
      {"permil", "\u2030"}, {"prime", "\u2032"},  {"Prime", "\u2033"},   {"lsaquo", "\u2039"},   {"rsaquo", "\u203a"},
      {"oline", "\u203e"},  {"frasl", "\u2044"},  {"euro", "\u20ac"},    {"image", "\u2111"},    {"weierp", "\u2118"},
      {"real", "\u211c"},   {"trade", "\u2122"},  {"alefsym", "\u2135"}, {"larr", "\u2190"},     {"uarr", "\u2191"},
      {"rarr", "\u2192"},   {"darr", "\u2193"},   {"harr", "\u2194"},    {"crarr", "\u21b5"},    {"lArr", "\u21d0"},
      {"uArr", "\u21d1"},   {"rArr", "\u21d2"},   {"dArr", "\u21d3"},    {"hArr", "\u21d4"},     {"forall", "\u2200"},
      {"part", "\u2202"},   {"exist", "\u2203"},  {"empty", "\u2205"},   {"nabla", "\u2207"},    {"isin", "\u2208"},
      {"notin", "\u2209"},  {"ni", "\u220b"},     {"prod", "\u220f"},    {"sum", "\u2211"},      {"minus", "\u2212"},
      {"lowast", "\u2217"}, {"radic", "\u221a"},  {"prop", "\u221d"},    {"infin", "\u221e"},    {"ang", "\u2220"},
      {"and", "\u2227"},    {"or", "\u2228"},     {"cap", "\u2229"},     {"cup", "\u222a"},      {"int", "\u222b"},
      {"there4", "\u2234"}, {"sim", "\u223c"},    {"cong", "\u2245"},    {"asymp", "\u2248"},    {"ne", "\u2260"},
      {"equiv", "\u2261"},  {"le", "\u2264"},     {"ge", "\u2265"},      {"sub", "\u2282"},      {"sup", "\u2283"},
      {"nsub", "\u2284"},   {"sube", "\u2286"},   {"supe", "\u2287"},    {"oplus", "\u2295"},    {"otimes", "\u2297"},
      {"perp", "\u22a5"},   {"sdot", "\u22c5"},   {"lceil", "\u2308"},   {"rceil", "\u2309"},    {"lfloor", "\u230a"},
      {"rfloor", "\u230b"}, {"lang", "\u2329"},   {"rang", "\u232a"},    {"loz", "\u25ca"},      {"spades", "\u2660"},
      {"clubs", "\u2663"},  {"hearts", "\u2665"}, {"drvps", "\u2666"}};

  auto s = entities.find(entity);
  return (s != entities.end()) ? s->second : "";
}

uint64_t stringToHex(const std::string &s) {

  static const std::regex rx{"[[:xdigit:]]+"};
  if (!std::regex_match(s, rx)) {
    throw std::runtime_error(s + " is not correct hex number.");
  }
  return std::stoull(s, nullptr, 16);
}

std::string HtmlDecode(const std::string &s) {
  if (s.empty() || !stringRequiresHtmlDecoding(s)) {
    return s;
  }

  std::stringstream ss;

  for (size_t i = 0; i < s.length(); ++i) {
    if (s[i] == '&') {
      // We found a '&'. Now look for the next ';' or '&'. The idea is that
      // if we find another '&' before finding a ';', then this is not an
      // entity,
      // and the next '&' might start a real entity (VSWhidbey 275184)
      auto pos = s.find_first_of(";&", i + 1);
      if (pos != std::string::npos && s[pos] == ';') {
        auto entity = s.substr(i + 1, pos - i - 1);

        if (entity.length() > 1 && entity[0] == '#') {

          // The # syntax can be in decimal or hex, e.g.
          //      &#229;  --> decimal
          //      &#xE5;  --> same char in hex
          // See http://www.w3.org/TR/REC-html40/charset.html#entities

          auto parsedValue =
              (entity[1] == 'x' || entity[1] == 'X') ? stringToHex(entity.substr(2)) : std::stoll(entity.substr(1));
          // decoded character must be U+0000 .. U+10FFFF, excluding surrogates
          if ((parsedValue < HIGH_SURROGATE_START) ||
              ((LOW_SURROGATE_END < parsedValue) && parsedValue <= UNICODE_PLANE16_END)) {

            if (parsedValue <= 0x7F) {
              ss << static_cast<char>(parsedValue);
            } else if (parsedValue <= 0x7FF) {
              ss << static_cast<char>(0xC0 | (parsedValue >> 6));
              ss << static_cast<char>(0x80 | (parsedValue & 0x3F));
            } else if (parsedValue <= UNICODE_PLANE00_END) {
              ss << static_cast<char>(0xE0 | (parsedValue >> 12));
              ss << static_cast<char>(0x80 | ((parsedValue >> 6) & 0x3F));
              ss << static_cast<char>(0x80 | (parsedValue & 0x3F));
            } else {
              ss << static_cast<char>(0xF0 | (parsedValue >> 18));
              ss << static_cast<char>(0x80 | ((parsedValue >> 12) & 0x3F));
              ss << static_cast<char>(0x80 | ((parsedValue >> 6) & 0x3F));
              ss << static_cast<char>(0x80 | (parsedValue & 0x3F));
            }

            i = pos;
            continue;
          }

        } else {
          i = pos;
          auto htmlEntity = htmlEntitiesLookup(entity);
          if (!htmlEntity.empty()) {
            ss << htmlEntity;
          } else {
            ss << '&';
            ss << entity;
            ss << ';';
          }
          continue;
        }
      }
    }
    ss << s[i];
  }

  return ss.str();
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
      ss << encode(s[i]);
    }
  }

  return ss.str();
}

int hexToInt(char h) {
  return (h >= '0' && h <= '9') ? h - '0'
                                : (h >= 'a' && h <= 'f') ? h - 'a' + 10 : (h >= 'A' && h <= 'F') ? h - 'A' + 10 : -1;
}

std::string UrlDecode(const std::string &s) {
  if (s.empty()) {
    return s;
  }
  std::stringstream ss;
  for (size_t i = 0; i < s.length(); ++i) {

    char ch = s[i];

    if (ch == '+') {
      ch = ' ';
    } else if ((ch == '%') && (i < (s.length() - 2))) {
      auto h1 = hexToInt(s[i + 1]);
      auto h2 = hexToInt(s[i + 2]);

      if (h1 >= 0 && h2 >= 0) {
        ss << static_cast<char>((h1 << 4) | h2);
        i += 2;
        continue;
      }
    }

    ss << ch;
  }

  return ss.str();
}
} // namespace utils
