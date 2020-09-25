#include <iostream>
#include <string>
#include <vector>

long long countPalindromeNumber(const std::string& data);
std::vector<long long> countPalindromeNumber(const std::string& data, bool countEvenNumber);

int main() {
  std::string line;
  std::cin >> line;
  std::cout << countPalindromeNumber(line) << std::endl;
}

/**
* Находит количество палиндромов длины хотя бы 2 в строке @data
*/
long long countPalindromeNumber(const std::string& data) {
  long long result = -data.length(); // не учитываются палиндромы длины 1
  for (auto el: countPalindromeNumber(data, true)) {
    result += el;
  }
  for (auto el: countPalindromeNumber(data, false)) {
    result += el;
  }
  return result;
}

/**
* Для @result[i] считает количество полиндромов нечетной длины,
* если @countEvenNumber == true, и четных иначе
* с центром в @data[i]
*/
std::vector<long long> countPalindromeNumber(const std::string& data, bool countEvenNumber) {
  int length = data.length();
  std::vector<long long> result(length);
  long long leftEnd = 0;
  long long rightEnd = -1;
  for (int i = 0; i < length; ++i) {
    long long counter = 0;
    if (i <= rightEnd) {
      counter = std::min(rightEnd - i + 1, result[rightEnd + leftEnd - i + countEvenNumber]);
    }
    while (i + counter < length && i - counter >= 0 && data[i + counter] == data[i - counter - countEvenNumber]) {
      ++counter;
    }
    result[i] = counter;
    if (i + counter - 1 > rightEnd) {
      leftEnd = i - counter + 1 - countEvenNumber;
      rightEnd = i + counter - 1;
    }
  }
  return result;
}