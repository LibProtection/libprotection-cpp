#include "Languages/Url.h"

#include <numeric>
#include <stdexcept>

namespace protection {

const std::vector<RegexTokenDefinition> &Url::getTokenDefinitions() const {
  static const std::vector<RegexTokenDefinition> tokenDefinitions = {{R"([^:/?#]+:)", TokenType::UrlSchemeCtx},
                                                                     {R"(//[^/?#]*)", TokenType::UrlAuthorityCtx},
                                                                     {R"([^?#]*)", TokenType::UrlPathCtx},
                                                                     {R"(\?[^#]*)", TokenType::UrlQueryCtx},
                                                                     {R"(#.*)", TokenType::UrlFragmentCtx}};
  return tokenDefinitions;
}

std::vector<Token> Url::tokenize(const std::string &text, size_t offset) {
  std::vector<Token> tokens;
  for (const auto &token : RegexLanguageProvider::tokenize(text, offset)) {
    auto tokenText = token.text;
    auto lowerBound = token.range.lowerBound;

    switch (token.tokenType) {
    case TokenType::UrlSchemeCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, ":", TokenType::UrlScheme)) {
        tokens.push_back(subToken);
      }
      break;

    case TokenType::UrlAuthorityCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "\\/:@", TokenType::UrlAuthorityEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case TokenType::UrlPathCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "\\/", TokenType::UrlPathEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case TokenType::UrlQueryCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "?&=", TokenType::UrlQueryEntry)) {
        tokens.push_back(subToken);
      }
      break;

    case TokenType::UrlFragmentCtx:
      for (const auto &subToken : splitToken(tokenText, lowerBound, "#", TokenType::UrlFragment)) {
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

TokenType Url::getErrorTokenType() const { return TokenType::UrlError; }

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

      tokens.push_back(createToken(TokenType::UrlSeparator, lowerBound, lowerBound, std::string{ch}));
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
  case TokenType::UrlPathEntry: {
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
  case TokenType::UrlQueryEntry:
  case TokenType::UrlFragment:
    encodedText = UrlEncode(text);
    return {encodedText, true};

  default:
    throw std::runtime_error{"Unsupported Url Token Type:"};
  }

  return {encodedText, false};
}
} // namespace protection