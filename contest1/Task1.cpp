#include <iostream>
#include <string>
#include <vector>

void countPrefixFunction(std::string& string, std::vector<int>& answer) {
  ///  Вычисляем префикс-функцию
  answer.push_back(0);
  for (int i = 1; i < string.length(); ++i) {
    int borderLength = answer[i - 1];
    while (borderLength > 0 && string[i] != string[borderLength]) {
      borderLength = answer[borderLength - 1];
    }
    if (string[borderLength] == string[i]) {
      ++borderLength;
    }
    answer.push_back(borderLength);
  }
}

int main() {
  std::string line;
  std::string pattern;
  std::cin >> pattern >> line;
  std::string united = pattern + "#" + line;
  std::vector<int> prefixFunction = std::vector<int>();
  countPrefixFunction(united, prefixFunction);
  std::vector<int> answer = std::vector<int>();
  for (int i = 0; i < line.length(); ++i) {
    if (prefixFunction[pattern.length() + i + 1] == pattern.length()) {
      // вычитается длина паттерна
      answer.push_back(i - prefixFunction[pattern.length() + i + 1] + 1);
    }
  }
  for (auto element: answer) {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}
