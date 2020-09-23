#include <iostream>
#include <string>
#include <vector>

long long countPalindromeNumber(const std::string& data);
std::vector<long long> countOddPalindromes(const std::string& data);
std::vector<long long> countEvenPalindromes(const std::string& data);

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
  for (auto el: countEvenPalindromes(data)) {
    result += el;
  }
  for (auto el: countOddPalindromes(data)) {
    result += el;
  }
  return result;
}

/**
* Для @result[i] считает количество полиндромов нечетной длины
* с центром в @data[i], в том числе односимвольные палиндромы
*/
std::vector<long long> countOddPalindromes(const std::string& data) {
  std::vector<long long> result(data.length());
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
  return result;
}

/**
 * Для result[i] считает количество палиндромов
 * четной длины с центром между @data[i-1] и @data[i]
 */
std::vector<long long> countEvenPalindromes(const std::string& data) {
  std::vector<long long> result(data.length());
  long long leftEnd = 0;
  long long rightEnd = -1;
  for (int i = 0; i < data.length(); ++i) {
    long long counter = 0;
    if (i <= rightEnd) {
      counter = std::min(rightEnd - i + 1, result[rightEnd + leftEnd - i + 1]);
    }
    while (i + counter < data.length() && i - counter > 0 && data[i + counter] == data[i - counter - 1]) {
      ++counter;
    }
    result[i] = counter;
    if (i + counter - 1 > rightEnd) {
      leftEnd = i - counter;
      rightEnd = i + counter - 1;
    }
  }
  return result;
}
