#include <iostream>
#include <string>
#include <vector>

#include "PatternMatching.h"

PatternMatching::PatternMatching(const std::string& data, const std::string& pattern)
    : data(data), pattern(pattern), dataLength(data.length()), patternLength(pattern.length()) {
  prefixFunction.reserve(data.length());
  countPrefixFunction(this->pattern + DEFAULT_PATTERN_SEPARATOR + this->data, prefixFunction);
}

void PatternMatching::findPattern() {
  for (int i = 0; i < dataLength; ++i) {
    if (prefixFunction[patternLength + i + 1] == patternLength) {
      // вычитается длина паттерна
      patternPositions.emplace_back(i - prefixFunction[patternLength + i + 1] + 1);
    }
  }
}

void PatternMatching::countPrefixFunction(std::string&& string, std::vector<int>& answer) {
  ///  Вычисляет префикс-функцию
  answer.emplace_back(0);
  for (int i = 1; i < string.length(); ++i) {
    int borderLength = answer[i - 1];
    while (borderLength > 0 && string[i] != string[borderLength]) {
      borderLength = answer[borderLength - 1];
    }
    if (string[borderLength] == string[i]) {
      ++borderLength;
    }
    answer.emplace_back(borderLength);
  }
}

const std::vector<int>& PatternMatching::getPatternPositions() const {
  return patternPositions;
}
