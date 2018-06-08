#include "protection/LanguageService.h"

#include <algorithm>

namespace protection {
namespace injections {

SanitizeResult::SanitizeResult(bool sanitized, std::vector<Token> t, std::string sanitizedStr, Token token)
    : success{sanitized}, tokens{std::move(t)}, sanitizedString{sanitizedStr}, attackToken{token} {}

bool LanguageService::TokenScope::isTrivial() const {
  return std::all_of(tokens.begin(), tokens.end(), [](const Token &t) { return t.isTrivial; });
}

std::vector<LanguageService::TokenScope> LanguageService::getTokensScopes(const std::vector<Token> &tokens,
                                                                          const std::vector<Range> &ranges) {
  std::unordered_map<Range, TokenScope> scopesMap;
  for (const auto &token : tokens) {
    for (const auto &range : ranges) {
      if (range.overlaps(token.range)) {
        auto it = scopesMap.emplace(range, TokenScope{range}).first;
        it->second.tokens.push_back(token);
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
