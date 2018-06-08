#ifndef PROTECTION_LANGUAGESERVICE_H
#define PROTECTION_LANGUAGESERVICE_H

#include "protection/LanguageProvider.h"
#include "protection/Range.h"
#include "protection/Single.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

namespace protection {
namespace injections {

struct LIBPRROTECTION_EXPORT SanitizeResult {
  SanitizeResult(bool sanitized, std::vector<Token> t, std::string sanitizedStr, Token token);

  SanitizeResult(const SanitizeResult &) = default;
  SanitizeResult(SanitizeResult &&) = default;

  SanitizeResult &operator=(const SanitizeResult &) = default;
  SanitizeResult &operator=(SanitizeResult &&) = default;

  bool success{false};
  std::vector<Token> tokens;
  std::string sanitizedString;
  Token attackToken;
};

class LIBPRROTECTION_EXPORT LanguageService {
public:
  template <typename LP>
  static SanitizeResult trySanitize(const std::string &text, const std::vector<Range> &taintedRanges) {
    auto sanitizedRanges = std::vector<Range>{};
    auto languageProvider = Single<LP>::instance();
    auto tokens = languageProvider.tokenize(text);
    auto fragments = std::unordered_map<Range, std::string>{};

    // Try to sanitize all attacked text's fragments
    for (const auto &tokensScope : getTokensScopes(tokens, taintedRanges)) {
      auto range = tokensScope.range;
      auto fragment = text.substr(range.lowerBound, range.length());

      auto sanitized = languageProvider.trySanitize(fragment, tokensScope.tokens[0]);
      fragments.insert(std::make_pair(range, (sanitized.second) ? sanitized.first : fragment));
    }

    // Replace all attacked text's fragments with corresponding sanitized values
    size_t positionAtText{0};
    std::string sanitizedString;

    for (const auto &fragment : fragments) {
      auto charsToAppend = fragment.first.lowerBound - positionAtText;
      sanitizedString.append(text.substr(positionAtText, charsToAppend));
      auto lowerBound = sanitizedString.length();
      sanitizedString.append(fragment.second);
      sanitizedRanges.emplace_back(Range{lowerBound, sanitizedString.length() - 1});
      positionAtText = fragment.first.upperBound + 1;
    }

    if (positionAtText < text.length()) {
      sanitizedString.append(text.substr(positionAtText, text.length() - positionAtText));
    }

    auto validateResult = validate<LP>(sanitizedString, sanitizedRanges);
    return SanitizeResult(validateResult.second, tokens, (validateResult.second) ? sanitizedString : std::string{},
                          validateResult.first);
  }

  template <typename LP>
  static std::pair<Token, bool> validate(const std::string &text, const std::vector<Range> &ranges) {
    auto languageProvider = Single<LP>::instance();
    auto tokens = languageProvider.tokenize(text);
    auto scopesCount{0};
    auto allTrivial{true};

    for (const auto &scope : getTokensScopes(tokens, ranges)) {
      ++scopesCount;
      allTrivial = allTrivial && scope.isTrivial();
      if ((scope.tokens.size() > 1 || scopesCount > 1) && !allTrivial) {
        auto it = std::find_if(scope.tokens.begin(), scope.tokens.end(),
                               [](const Token &token) -> bool { return !token.isTrivial; });
        if (it != scope.tokens.end()) {
          return {*it, false};
        }
      }
    }

    return {Token(), true};
  }

private:
  struct LIBPRROTECTION_EXPORT TokenScope {
    Range range;
    std::vector<Token> tokens;

    bool isTrivial() const;

    explicit TokenScope(Range r) : range{r} {}
  };

  static std::vector<TokenScope> getTokensScopes(const std::vector<Token> &tokens, const std::vector<Range> &ranges);
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_LANGUAGESERVICE_H
