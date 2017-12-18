#ifndef PROTECTION_WEBUTILS_H
#define PROTECTION_WEBUTILS_H

#include <string>

namespace utility {

std::string HtmlAttributeEncode(const std::string &s);

std::string HtmlEncode(const std::string &s);

std::string UrlEncode(const std::string &s);

std::string JavaScriptStringEncode(const std::string& s);

} // namespace utils
#endif // PROTECTION_WEBUTILS_H
