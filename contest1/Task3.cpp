#include <iostream>
#include "Task3/AhoCorasickMachine.h"

int main() {
  std::string mask;
  std::string text;
  std::cin >> mask >> text;
  AhoCorasickMachine machine(mask);
  machine.workForText(text);
}
