#ifndef PROTECTION_LANGUAGESERVICE_H
#define PROTECTION_LANGUAGESERVICE_H

#include <LanguageProvider.h>
#include <Range.h>
#include <Single.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace protection {
namespace injections {

class LanguageService {
public:
  template <typename LP>
  static std::pair<std::string, bool> trySanitize(const std::string &text, const std::vector<Range> &taintedRanges) {
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
      sanitizedString.append(fragments[fragment.first]);
      sanitizedRanges.push_back(Range{lowerBound, sanitizedString.length() - 1});
      positionAtText = fragment.first.upperBound + 1;
    }

    if (positionAtText < text.length()) {
      sanitizedString.append(text.substr(positionAtText, text.length() - positionAtText));
    }

    return {sanitizedString, Validate<LP>(sanitizedString, sanitizedRanges)};
  }

  template <typename LP> static bool Validate(const std::string &text, const std::vector<Range> &ranges) {
    auto languageProvider = Single<LP>::instance();
    auto tokens = languageProvider.tokenize(text);
    auto scopesCount{0};
    auto allTrivial{true};

    for (const auto &scope : getTokensScopes(tokens, ranges)) {
      ++scopesCount;
      allTrivial = allTrivial && scope.isTrivial();
      if ((scope.tokens.size() > 1 || scopesCount > 1) && !allTrivial) {
        return false;
      }
    }

    return true;
  }

private:
  struct TokenScope {
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
