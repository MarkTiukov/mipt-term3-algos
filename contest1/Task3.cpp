#include <iostream>
#include "Task3/AhoCorasickMachine.h"

void inputData(std::string& mask, std::string& text) {
  std::cin >> mask >> text;
}

int main() {
  std::string mask;
  std::string text;
  inputData(mask, text);
  AhoCorasickMachine machine(mask);
  machine.printAllOccurrences(text);
}
