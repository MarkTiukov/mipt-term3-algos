#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

static const int ALPHABET_SIZE = 26;

struct Node {
  std::unordered_map<char, std::shared_ptr<Node>> sons = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// сыновья / исходящие связи
  std::unordered_map<char, std::shared_ptr<Node>> moves = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// переходы автомата
  std::shared_ptr<Node> suffixLink = nullptr; /// суффиксная ссылка
  std::shared_ptr<Node> up = nullptr; /// сжатая ссылка
  std::shared_ptr<Node> parent = nullptr; /// ссылка на родителя
  char parentChar; /// символ на переходе из родителя в текущую вершину
  bool isPatternEnd = false; /// является ли концом некоторого шаблона
  std::vector<int> patternIndexes; ///  шаблоны, с которыми связана данная вершина

  Node() : parentChar(0) {}
  Node(const std::shared_ptr<Node>& parent, char parentChar) : parent(parent), parentChar(parentChar) {}

};

class AhoCorasickMachine {
 private:
  const std::string mask;
  std::shared_ptr<Node> trieRoot;
  std::vector<int> startPositions;
  std::vector<int> lengths;

  std::shared_ptr<Node> getMove(std::shared_ptr<Node> node, char symbol);
  std::shared_ptr<Node> makeLink(std::shared_ptr<Node> node);
  std::shared_ptr<Node> makeUP(std::shared_ptr<Node> node);
  void buildTrie();
 public:
  AhoCorasickMachine(const std::string& mask);
  void printTrie(std::shared_ptr<Node> node);
  void printTrie();
  void printStarts();
  std::shared_ptr<Node> goBySymbol(std::vector<int>& counter, std::shared_ptr<Node> node);
  void workForText(std::string& text);
};

int main() {
  std::string mask;
  std::string text;
  std::cin >> mask >> text;
  AhoCorasickMachine machine(mask);
  machine.workForText(text);
}

AhoCorasickMachine::AhoCorasickMachine(const std::string& mask) : mask(mask), trieRoot(std::make_shared<Node>()) { buildTrie(); }

void AhoCorasickMachine::buildTrie() {
  std::shared_ptr<Node> currentNode = trieRoot;
  int patternNumber = 0;
  bool hasStared = true;
  int currentLength = 0;
  for (int i = 0; i < mask.length(); ++i) {
    if (currentNode != trieRoot && mask[i] == '?') {
      if (currentLength > 0) {
        lengths.push_back(currentLength);
        currentLength = 0;
      }
      hasStared = true;
      currentNode->isPatternEnd = true;
      currentNode->patternIndexes.push_back(patternNumber);
      patternNumber++;
      currentNode = trieRoot;
    } else {
      if (mask[i] == '?') {
        if (currentLength > 0) {
          lengths.push_back(currentLength);
          currentLength = 0;
        }
        continue;
      }
      if (hasStared) {
        hasStared = false;
        startPositions.push_back(i);
      }
      ++currentLength;
      std::shared_ptr<Node> newNode = std::make_shared<Node>(currentNode, mask[i]);
      currentNode->sons.try_emplace(mask[i], newNode);
      currentNode = currentNode->sons[mask[i]];
    }
  }
  if (currentLength > 0) {
    lengths.push_back(currentLength);
    currentLength = 0;
  }
  currentNode->isPatternEnd = true;
  currentNode->patternIndexes.push_back(patternNumber);
}

std::shared_ptr<Node> AhoCorasickMachine::makeLink(std::shared_ptr<Node> node) {
  if (node->suffixLink == nullptr) {
    if (node == trieRoot || node->parent == trieRoot) {
      node->suffixLink = trieRoot;
    } else {
      node->suffixLink = getMove(makeLink(node->parent), node->parentChar);
    }
  }
  return node->suffixLink;
}
std::shared_ptr<Node> AhoCorasickMachine::getMove(std::shared_ptr<Node> node, char symbol) {
  if (node->moves.find(symbol) == node->moves.end()) {
    if (node->sons.find(symbol) != node->moves.end()) {
      node->moves.emplace(symbol, node->sons[symbol]);
    } else {
      if (node == trieRoot) {
        node->moves.emplace(symbol, trieRoot);
      } else {
        node->moves.emplace(symbol, getMove(makeLink(node), symbol));
      }
    }
  }
  return node->moves[symbol];
}
std::shared_ptr<Node> AhoCorasickMachine::makeUP(std::shared_ptr<Node> node) {
  if (node->up == nullptr) {
    auto currentSuffixLink = makeLink(node);
    if (currentSuffixLink->isPatternEnd) {
      node->up = currentSuffixLink;
    } else {
      if (currentSuffixLink == trieRoot) {
        node->up = trieRoot;
      } else {
        node->up = makeUP(currentSuffixLink);
      }
    }
  }
  return node->up;
}
void AhoCorasickMachine::printTrie() {
  printTrie(trieRoot);
}
void AhoCorasickMachine::printTrie(std::shared_ptr<Node> node) {
  std::cout << std::endl << "<started>" << std::endl;
  for (auto son: node->sons) {
    std::cout << "from " << son.first;
    printTrie(son.second);
  }
  std::cout << "<left>" << std::endl;
}

void AhoCorasickMachine::printStarts() {
  for (auto el: startPositions) {
    std::cout << el << " ";
  }
  std::cout << std::endl;
}

void AhoCorasickMachine::workForText(std::string& text) {
  std::vector<int> counter(text.length());
  std::shared_ptr<Node> currentNode = trieRoot;
  int begin = 0;
  for (int i = 0; i < text.length(); i++) {
    currentNode = getMove(currentNode, text[i]);
    std::shared_ptr<Node> temporaryNode = currentNode;
    while (temporaryNode != trieRoot) {
      if (temporaryNode->isPatternEnd) {
        for (auto index: temporaryNode->patternIndexes) {
//          std::cout << i << " " << startPositions[index] << " " << index << std::endl;
          ++counter[i - lengths[index] - startPositions[index] + 1];
        }
      }
      temporaryNode = makeUP(temporaryNode);
    }
    begin = i + 1;
  }
  for (int i = 0; i < counter.size(); ++i) {
    if (counter[i] == startPositions.size())
      std::cout << i << " ";
//    std::cout << counter[i] << " ";
  }
  std::cout << std::endl;

//  for (auto el: startPositions) {
//    std::cout << el << " ";
//  }
//  std::cout << std::endl;
//
//  for (auto el: lengths) {
//    std::cout << el << " ";
//  }
//  std::cout << std::endl;
}
