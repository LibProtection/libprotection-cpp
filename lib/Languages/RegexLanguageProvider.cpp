#include "RegexLanguageProvider.h"

namespace protection {

std::vector<Token> RegexLanguageProvider::tokenize(const std::string &text, size_t offset) {
  size_t currentPosition{0};

  std::vector<Token> tokens;

  std::string str = text;

  while (!str.empty()) {
    bool isMatched{false};

    for (const auto &tokenDefinition : getTokenDefinitions()) {
      size_t matchedLength;
      auto m = tokenDefinition.tryMatch(str);
      if (m.second && (m.first != 0)) {
        auto matchedLength = m.first;
        isMatched = true;
        auto tokenText = str.substr(0, matchedLength);

        auto token = createToken(TokenType::FilePathFSEntryName, currentPosition + offset,
                                 currentPosition + offset + tokenText.length() - 1, tokenText);

        str = str.substr(matchedLength);
        currentPosition += matchedLength;

        tokens.push_back(token);
        break;
      }
    }
    // Simply error-tolerance strategy: consider current char as error-token and
    // move to next
    if (!isMatched) {
      auto token =
          createToken(TokenType::ErrorTokenType, currentPosition + offset, currentPosition + offset, str.substr(0, 1));

      str = str.substr(1);
      ++currentPosition;

      tokens.push_back(token);
    }
  }
}
}