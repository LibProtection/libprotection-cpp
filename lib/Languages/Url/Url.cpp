#include "Languages/Url.h"

#include <numeric>
#include <stdexcept>

namespace protection {

const std::vector<RegexTokenDefinition> &Url::getTokenDefinitions() const {
  static const std::vector<RegexTokenDefinition> tokenDefinitions = {{R"([^:/?#]+:)", UrlTokenType::SchemeCtx},
                                                                     {R"(//[^/?#]*)", UrlTokenType::AuthorityCtx},
                                                                     {R"([^?#]*)", UrlTokenType::PathCtx},
                                                                     {R"(\?[^#]*)", UrlTokenType::QueryCtx},
                                                                     {R"(#.*)", UrlTokenType::FragmentCtx}};
  return tokenDefinitions;
}

std::vector<Token> Url::tokenize(const std::string &text, size_t offset) {
  std::vector<Token> tokens;
  for (const auto &token : RegexLanguageProvider::tokenize(text, offset)) {
    auto tokenText = token.text;
    auto lowerBound = token.range.lowerBound;

    switch (token.tokenType) {
    case UrlTokenType::SchemeCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, ":", UrlTokenType::Scheme)) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::AuthorityCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "\\/:@", UrlTokenType::AuthorityEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::PathCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "\\/", UrlTokenType::PathEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::QueryCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "?&=", UrlTokenType::QueryEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case UrlTokenType::FragmentCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "#", UrlTokenType::Fragment)) {
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

bool Url::isTrivial(TokenType type, const std::string &text) const { return true; }

TokenType Url::getErrorTokenType() const { return UrlTokenType::Error; }

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

      tokens.push_back(createToken(UrlTokenType::Separator, lowerBound, lowerBound, std::string{ch}));
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
  switch (tokenType) {
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
        fragment = UrlEncode(fragment);
      }
    }

    auto beginIt = std::begin(fragments);
    auto endIt = std::end(fragments);

    if (beginIt != endIt) {
      encodedText.insert(std::end(encodedText), std::begin(*beginIt), std::end(*beginIt));
      ++beginIt;
    }
    for (; beginIt != endIt; ++beginIt) {
      encodedText.insert(std::end(encodedText), {'/'});
      encodedText.insert(std::end(encodedText), std::begin(*beginIt), std::end(*beginIt));
    }

    return {encodedText, true};
  }
  case UrlTokenType::QueryEntry:
  case UrlTokenType::Fragment:
    encodedText = UrlEncode(text);
    return {encodedText, true};

  default:
    break;
  }

  return {encodedText, false};
}
} // namespace protection