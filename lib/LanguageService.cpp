#include <LanguageService.h>

#include <algorithm>

namespace protection {
namespace injections {

bool LanguageService::TokenScope::isTrivial() const {
  return std::all_of(tokens.begin(), tokens.end(), [](const Token &t) { return t.isTrivial; });
}

std::vector<LanguageService::TokenScope> LanguageService::getTokensScopes(const std::vector<Token> &tokens,
                                                                          const std::vector<Range> &ranges) {
  std::unordered_map<Range, TokenScope> scopesMap;
  for (const auto &token : tokens) {
    for (const auto &range : ranges) {
      if (range.overlaps(token.range)) {
        auto search = scopesMap.find(range);
        if (search == scopesMap.end()) {
          scopesMap.emplace(range, TokenScope{range});
        }
        search->second.tokens.push_back(token);
      }
    }
  }
  std::vector<TokenScope> scopes;
  for (const auto &s : scopesMap) {
    scopes.push_back(s.second);
  }

  return scopes;
}

} // namespace injections
} // namespace protection
