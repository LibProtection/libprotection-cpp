#include "RegexLanguageProvider.h"

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

    for (auto &rule : modeRuleStack.top()) {
      auto match = rule.tryMatch(str);
      if (match.second && (match.first != 0)) {
        auto matchedLength = match.first;
        isMatched = true;
        if (rule.isToken()) {
          auto tokenText = str.substr(0, matchedLength);

          auto token = createToken(rule.getType(), currentPosition + offset,
                                   currentPosition + offset + tokenText.length() - 1, tokenText);

          str = str.substr(matchedLength);
          currentPosition += matchedLength;

          tokens.push_back(token);
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
      auto token =
          createToken(getErrorTokenType(), currentPosition + offset, currentPosition + offset, str.substr(0, 1));

      str = str.substr(1);
      ++currentPosition;

      tokens.push_back(token);
    }
  }
  return tokens;
}

} // namespace injections
} // namespace protection