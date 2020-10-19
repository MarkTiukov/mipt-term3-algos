#include <iostream>
#include <vector>
#include <string>

class SuffixMachine {
private:
    static const char LOW_SYMBOL = '$';
    static const size_t ALPHABET_SIZE = 256; // 'a'-'z', '$'
    std::string text;
    std::string specialText;
    std::vector<long long> suffixArray;
    std::vector<size_t> equalityClasses;
    std::vector<int> lcp;

    void buildLCP();
    void buildSuffixArray();
    void sortByClasses(std::vector<long long> newSuffixArray, size_t exponent, size_t equalityClassNumber);
    size_t findEqualityClasses();
    size_t findEqualityClasses(size_t phaseNumber);
    size_t log2(size_t n);

public:
    SuffixMachine(const std::string& text) : text(text), specialText(text + LOW_SYMBOL),
                                             suffixArray(specialText.length()), lcp(text.length()),
                                             equalityClasses(specialText.length()) {
        this->buildSuffixArray();
        this->buildLCP();
    }
    
    size_t countDifferentSubstrings();
};

std::string readData();

int main() {
    SuffixMachine machine(readData());
    std::cout << machine.countDifferentSubstrings() << std::endl;
}

std::string readData() {
    std::string data;
    std::cin >> data;
    return data;
}

void SuffixMachine::buildSuffixArray() {
    const size_t specialTextLength = specialText.length();
    size_t equalityClassNumber = findEqualityClasses();
    std::vector<long long> newSuffixMassiv(specialTextLength);
    for (int i = 0; (1 << i) < specialTextLength; ++i) {
        for (int j = 0; j < specialTextLength; ++j) {
            newSuffixMassiv[j] = suffixArray[j] - (1 << i);
            if (newSuffixMassiv[j] < 0) {
                newSuffixMassiv[j] += specialTextLength;
            }
        }
        sortByClasses(newSuffixMassiv, i, equalityClassNumber);
        equalityClassNumber = findEqualityClasses(i);
    }
    suffixArray.erase(suffixArray.begin());
}

size_t SuffixMachine::findEqualityClasses() {
    std::vector<size_t> counters(ALPHABET_SIZE);
    for (auto letter : specialText) {
        ++counters[letter];
    }
    for (size_t i = 1; i < ALPHABET_SIZE; ++i) {
        counters[i] += counters[i - 1];
    }
    for (size_t i = 0; i < suffixArray.size(); ++i) {
        suffixArray[--counters[specialText[i]]] = i;
    }
    equalityClasses[suffixArray[0]] = 0;
    size_t equalityClassNumber = 1;
    for (size_t i = 1; i < specialText.length(); ++i) {
        if (specialText[suffixArray[i]] != specialText[suffixArray[i - 1]])
            ++equalityClassNumber;
        equalityClasses[suffixArray[i]] = equalityClassNumber - 1;
    }
    return equalityClassNumber;
}

size_t SuffixMachine::findEqualityClasses(size_t phaseNumber) {
    size_t specialTextLength = specialText.length();
    std::vector<size_t> newEqualityClasses(specialTextLength);
    size_t currentClassNumber = 1;
    for (size_t i = 1; i < specialTextLength; ++i) {
        size_t middle1 = (suffixArray[i] + (1 << phaseNumber)) % (specialTextLength);
        size_t middle2 = (suffixArray[i - 1] + (1 << phaseNumber)) % (specialTextLength);
        if (equalityClasses[suffixArray[i]] != equalityClasses[suffixArray[i - 1]] ||
            equalityClasses[middle1] != equalityClasses[middle2])
            ++currentClassNumber;
        newEqualityClasses[suffixArray[i]] = currentClassNumber - 1;
    }
    equalityClasses = newEqualityClasses;
    return currentClassNumber;
}

void SuffixMachine::sortByClasses(std::vector<long long> substrings, size_t exponent, size_t equalityClassNumber) {
    std::vector<size_t> counters(equalityClassNumber);
    for (size_t i = 0; i < specialText.length(); ++i) {
        ++counters[equalityClasses[substrings[i]]];
    }
    for (size_t i = 1; i < equalityClassNumber; ++i) {
        counters[i] += counters[i - 1];
    }
    for (size_t i = specialText.length(); i > 0; --i) {
        suffixArray[--counters[equalityClasses[substrings[i - 1]]]] = substrings[i - 1];
    }
}

void SuffixMachine::buildLCP() {
    std::vector<size_t> inverseSuffixArray(suffixArray.size());
    for (size_t i = 0; i < suffixArray.size(); ++i) {
        inverseSuffixArray[suffixArray[i]] = i;
    }
    int currentLCP = 0;
    for (size_t i = 0; i < suffixArray.size(); ++i) {
        if (currentLCP > 0) {
            --currentLCP;
        }
        if (inverseSuffixArray[i] == suffixArray.size() - 1) {
            lcp[suffixArray.size() - 1] = -1;
            currentLCP = 0;
        } else {
            size_t j = suffixArray.at(inverseSuffixArray[i] + 1);
            while (std::max(i, j) + currentLCP < suffixArray.size() &&
                   text[i + currentLCP] == text[j + currentLCP]) {
                ++currentLCP;
            }
            lcp[inverseSuffixArray[i]] = currentLCP;
        }
    }
    lcp.pop_back();
}

size_t SuffixMachine::countDifferentSubstrings() {
    size_t result = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        result += text.size() - suffixArray[i];
    }
    for (size_t i = 0; i < text.length() - 1; ++i) {
        result -= lcp[i];
    }
    return result;
}
    
