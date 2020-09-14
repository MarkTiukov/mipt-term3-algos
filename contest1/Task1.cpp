#include <iostream>
#include <string>
#include <vector>

void countPrefixFunction(std::string& string, std::vector<int>& answer) {
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
  for (auto el: prefixFunction) {
    std::cout << el << " " << std::endl;
  }
}
