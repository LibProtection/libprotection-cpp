#include "Languages/Url.h"
#include "Support/WebUtils.h"

#include <numeric>

namespace protection {

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
      RegexRule::Token("\\?", TOKEN_TYPE(UrlTokenType::Separator)),
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

std::vector<Token> Url::tokenize(const std::string &text, size_t offset) {
  std::vector<Token> tokens;
  for (const auto &token : RegexLanguageProvider::tokenize(text, offset)) {
    auto tokenText = token.text;
    auto lowerBound = token.range.lowerBound;

    switch (static_cast<UrlTokenType>(token.tokenType)) {
    case UrlTokenType::SchemeCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, ":", TOKEN_TYPE(UrlTokenType::Scheme))) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::AuthorityCtx:
      for (const auto &subToken :
           splitToken(tokenText, lowerBound, "\\/:@", TOKEN_TYPE(UrlTokenType::AuthorityEntry))) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::PathCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "\\/", TOKEN_TYPE(UrlTokenType::PathEntry))) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::QueryCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "?&=", TOKEN_TYPE(UrlTokenType::QueryEntry))) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::FragmentCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "#", TOKEN_TYPE(UrlTokenType::Fragment))) {
        tokens.push_back(subToken);
      }
      break;

    default:
      tokens.push_back(token);
      break;
    }
  }
  return {};
}

Token Url::createToken(TokenType type, size_t lowerBound, size_t upperBound, const std::string &text) const {
  return Token(LanguageProviderType::Url, type, lowerBound, upperBound, text, isTrivial(type, text));
}

std::pair<std::string, bool> Url::trySanitize(const std::string &text, Token context) {

  switch (context.languageProviderType) {
  case LanguageProviderType::Url: {
    auto encodeResult = tryUrlEncode(text, context.tokenType);
    if (encodeResult.second) {
      return encodeResult;
    }
    break;
  }
  default:
    throw std::runtime_error{std::string{"Unsupported URL island: "} + context.toString()};
  }

  return {{}, true};
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

std::vector<Token> Url::splitToken(const std::string &text, size_t lowerBound, const std::string &splitChars,
                                   TokenType tokenType) {
  if (text.empty()) {
    return {};
  }

  std::vector<Token> tokens;
  std::string lastTokenText;

  for (const auto &ch : text) {
    if (splitChars.find(ch) != std::string::npos) {
      if (!lastTokenText.empty()) {
        auto upperBound = lowerBound + lastTokenText.length() - 1;
        tokens.push_back(createToken(tokenType, lowerBound, upperBound, lastTokenText));
        lowerBound = upperBound + 1;
        lastTokenText.clear();
      }

      tokens.push_back(createToken(TOKEN_TYPE(UrlTokenType::Separator), lowerBound, lowerBound, std::string{ch}));
      ++lowerBound;

    } else {
      lastTokenText.push_back(ch);
    }
  }

  if (!lastTokenText.empty()) {
    tokens.push_back(createToken(tokenType, lowerBound, lowerBound + lastTokenText.length() - 1, lastTokenText));
  }

  return tokens;
}

std::pair<std::string, bool> Url::tryUrlEncode(const std::string &text, TokenType tokenType) {
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
        fragment = utils::UrlEncode(fragment);
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
    encodedText = utils::UrlEncode(text);
    return {encodedText, true};

  default:
    break;
  }

  return {encodedText, false};
}
} // namespace protection