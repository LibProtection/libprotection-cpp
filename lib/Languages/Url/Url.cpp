#include "protection/Languages/Url.h"
#include "support/WebUtils.h"

#include <map>
#include <numeric>

namespace protection {
namespace injections {

const std::vector<RegexRule> &Url::getMainModeRules() const {

  static const std::vector<RegexRule> schemeModeRules = {
      RegexRule::Token("[^:]+", TOKEN_TYPE(UrlTokenType::Scheme)),
      RegexRule::TokenPopMode(":", TOKEN_TYPE(UrlTokenType::Separator))};

  static const std::vector<RegexRule> authorityModeRules = {
      RegexRule::Token("//", TOKEN_TYPE(UrlTokenType::Separator)),
      RegexRule::Token("[^/@:]+", TOKEN_TYPE(UrlTokenType::AuthorityEntry)),
      RegexRule::Token("[:@]", TOKEN_TYPE(UrlTokenType::Separator)), RegexRule::NoTokenPopMode("/")};

  static const std::vector<RegexRule> pathModeRules = {RegexRule::Token("/", TOKEN_TYPE(UrlTokenType::Separator)),
                                                       RegexRule::Token("[^/?#]+", TOKEN_TYPE(UrlTokenType::PathEntry)),
                                                       RegexRule::NoTokenPopMode("[?#]")};

  static const std::vector<RegexRule> queryModeRules = {
      RegexRule::Token(R"(\?)", TOKEN_TYPE(UrlTokenType::Separator)),
      RegexRule::Token("[^?/=&#]+", TOKEN_TYPE(UrlTokenType::QueryEntry)),
      RegexRule::Token("[=&]", TOKEN_TYPE(UrlTokenType::Separator)), RegexRule::NoTokenPopMode("#")};

  static const std::vector<RegexRule> fragmentModeRules = {
      RegexRule::Token("#", TOKEN_TYPE(UrlTokenType::Separator)),
      RegexRule::TokenPopMode("[^#]*", TOKEN_TYPE(UrlTokenType::Fragment))};

  static const std::vector<RegexRule> mainModeRules = {RegexRule::NoTokenPushMode(R"([^:/?#]+:)", schemeModeRules),
                                                       RegexRule::NoTokenPushMode(R"(//[^/?#]*)", authorityModeRules),
                                                       RegexRule::NoTokenPushMode(R"([^?#]*)", pathModeRules),
                                                       RegexRule::NoTokenPushMode(R"(\?[^#]*)", queryModeRules),
                                                       RegexRule::NoTokenPushMode(R"(#.*)", fragmentModeRules)};

  return mainModeRules;
}

Token Url::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(this, type, lowerBound, upperBound, text, isTrivial(type, text));
}

std::pair<std::string, bool> Url::trySanitize(const std::string &text, const Token &context) const {
  if (dynamic_cast<decltype(this)>(context.languageProvider) != nullptr) {
    auto encodeResult = tryUrlEncode(text, context.tokenType);
    if (encodeResult.second) {
      return encodeResult;
    }
  } else {
    throw std::runtime_error{std::string{"Unsupported URL island: "} + context.toString()};
  }

  return {{}, false};
}

bool Url::isTrivial(TokenType type, const std::string &text) const {
  switch (static_cast<UrlTokenType>(type)) {
  case UrlTokenType::QueryEntry:
  case UrlTokenType::Fragment:
    return true;
  case UrlTokenType::PathEntry:
    return (text.find("..") == std::string::npos);
  default:
    break;
  }
  return false;
}

TokenType Url::getErrorTokenType() const { return TOKEN_TYPE(UrlTokenType::Error); }

std::pair<std::string, bool> Url::tryUrlEncode(const std::string &text, TokenType tokenType) const {
  std::string encodedText;
  switch (static_cast<UrlTokenType>(tokenType)) {
  case UrlTokenType::PathEntry: {
    std::vector<std::string> fragments;
    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;

    while ((pos = text.find('/', pos)) != std::string::npos) {
      fragments.push_back(text.substr(prev_pos, pos - prev_pos));
      prev_pos = ++pos;
    }
    fragments.push_back(text.substr(prev_pos, pos - prev_pos));

    for (auto &fragment : fragments) {
      if (!fragment.empty()) {
        fragment = utility::UrlEncode(fragment);
      }
    }

    auto beginIt = std::begin(fragments);
    auto endIt = std::end(fragments);

    if (beginIt != endIt) {
      encodedText.insert(std::end(encodedText), std::begin(*beginIt), std::end(*beginIt));
      ++beginIt;
    }
    for (; beginIt != endIt; ++beginIt) {
      encodedText.insert(std::end(encodedText), '/');
      encodedText.insert(std::end(encodedText), std::begin(*beginIt), std::end(*beginIt));
    }

    return {encodedText, true};
  }
  case UrlTokenType::QueryEntry:
  case UrlTokenType::Fragment:
    encodedText = utility::UrlEncode(text);
    return {encodedText, true};

  default:
    break;
  }

  return {encodedText, false};
}

std::string Url::tokenTypeToString(TokenType type) const {
  static const std::map<UrlTokenType, std::string> tokenTypeToStringMap = {
      {UrlTokenType::Error, "Error"},
      {UrlTokenType::Separator, "Separator"},
      {UrlTokenType::Scheme, "Scheme"},
      {UrlTokenType::AuthorityEntry, "AuthorityEntry"},
      {UrlTokenType::PathEntry, "PathEntry"},
      {UrlTokenType::QueryEntry, "QueryEntry"},
      {UrlTokenType::Fragment, "Fragment"},
      {UrlTokenType::SchemeCtx, "SchemeCtx"},
      {UrlTokenType::AuthorityCtx, "AuthorityCtx"},
      {UrlTokenType::PathCtx, "PathCtx"},
      {UrlTokenType::QueryCtx, "QueryCtx"},
      {UrlTokenType::FragmentCtx, "FragmentCtx"}};

  auto search = tokenTypeToStringMap.find(static_cast<UrlTokenType>(type));
  if (search == tokenTypeToStringMap.end()) {
    throw std::runtime_error{"Undefined Url Token type" + std::to_string(type)};
  }
  return search->second;
}

} // namespace injections
} // namespace protection
