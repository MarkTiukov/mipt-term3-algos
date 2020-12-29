#include <iostream>
#include <string>
#include "PatternMatching.h"

int main() {
  std::string line;
  std::string pattern;
  std::cin >> pattern >> line;
  PatternMatching matcher(line, pattern);
  matcher.findPattern();
  for (auto el: matcher.getPatternPositions()) {
    std::cout << el << " ";
  }
  std::cout << std::endl;
}
