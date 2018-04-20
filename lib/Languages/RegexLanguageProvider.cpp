#include "protection/RegexLanguageProvider.h"

#include <stack>

namespace protection {
namespace injections {

std::vector<Token> RegexLanguageProvider::tokenize(const std::string &text, size_t offset) const {
  size_t currentPosition{0};

  std::vector<Token> tokens;

  std::stack<std::vector<RegexRule>> modeRuleStack;
  modeRuleStack.push(getMainModeRules());

  std::string str = text;

  while (!str.empty()) {
    bool isMatched{false};

    auto rules = modeRuleStack.top();
    for (auto &rule : rules) {
      auto match = rule.tryMatch(str);
      if (match.second && (match.first != 0)) {
        auto matchedLength = match.first;
        isMatched = true;
        if (rule.isToken()) {
          auto tokenText = str.substr(0, matchedLength);

          tokens.push_back(createToken(rule.getType(), currentPosition + offset,
                                       currentPosition + offset + tokenText.length() - 1, tokenText));

          str = str.substr(matchedLength);
          currentPosition += matchedLength;
        }

        if (rule.isPopMode()) {
          modeRuleStack.pop();
        }
        if (rule.isPushMode()) {
          modeRuleStack.push(rule.getModeRules());
        }
        break;
      }
    }
    // Simply error-tolerance strategy: consider current char as error-token and
    // move to next
    if (!isMatched) {
      tokens.push_back(
          createToken(getErrorTokenType(), currentPosition + offset, currentPosition + offset, str.substr(0, 1)));

      str = str.substr(1);
      ++currentPosition;
    }
  }
  return tokens;
}

} // namespace injections
} // namespace protection
