#include <iostream>
#include <string>
#include <vector>

void countOddPalindromes(std::string& data, std::vector<long long>& result) {
  long long leftEnd = 0;
  long long rightEnd = -1;
  for (int i = 0; i < data.length(); ++i) {
    long long counter = 0;
    if (i <= rightEnd) {
      counter = std::min(rightEnd - i + 1, result[rightEnd + leftEnd - i]);
    }
    while (i + counter < data.length() && i - counter >= 0 && data[i + counter] == data[i - counter]) {
      ++counter;
    }
    result[i] = counter;
    if (i + counter - 1 > rightEnd) {
      leftEnd = i - counter + 1;
      rightEnd = i + counter - 1;
    }
  }
}

void countEvenPalindromes(std::string& data, std::vector<long long>& result) {
  long long leftEnd = 0;
  long long rightEnd = -1;
  for (int i = 0; i < data.length(); ++i) {
    long long counter = 0;
    if (i <= rightEnd) {
      counter = std::min(rightEnd - i + 1, result[rightEnd + leftEnd - i + 1]);
    }
    while (i + counter < data.length() && i - counter - 1 >= 0 && data[i + counter] == data[i - counter - 1]) {
      ++counter;
    }
    result[i] = counter;
    if (i + counter - 1 > rightEnd) {
      leftEnd = i - counter;
      rightEnd = i + counter - 1;
    }
  }
}

long long countPalindromeNumber(std::string& data) {
  std::vector<long long> oddLengths(data.length());
  std::vector<long long> evenLengths(data.length());
  countOddPalindromes(data, oddLengths);
  countEvenPalindromes(data, evenLengths);
  long long result = -1 * data.length();
  for (auto el: evenLengths) {
    result += el;
  }
  for (auto el: oddLengths) {
    result += el;
  }
  return result;
}

int main() {
  std::string line;
  std::cin >> line;
  std::cout << countPalindromeNumber(line) << std::endl;
}