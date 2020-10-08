#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

static const int ALPHABET_SIZE = 26;

struct Node {
  std::unordered_map<char, std::shared_ptr<Node>> sons = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// сыновья / исходящие связи
  std::unordered_map<char, std::shared_ptr<Node>> moves = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// переходы автомата
  std::weak_ptr<Node> suffixLink = std::shared_ptr<Node>(nullptr); /// суффиксная ссылка
  std::weak_ptr<Node> up = std::shared_ptr<Node>(nullptr); /// сжатая ссылка
  std::weak_ptr<Node> parent = std::shared_ptr<Node>(nullptr); /// ссылка на родителя
  char parentChar; /// символ на переходе из родителя в текущую вершину
  bool isPatternEnd = false; /// является ли концом некоторого шаблона
  std::vector<int> patternIndexes; ///  шаблоны, с которыми связана данная вершина

  Node() : parentChar(0) {}
  Node(const std::weak_ptr<Node>& parent, char parentChar) : parent(parent), parentChar(parentChar) {}

};

class AhoCorasickMachine {
 private:
  const std::string mask;
  int dividingCharacterNumber = 0;
  std::shared_ptr<Node> trieRoot;
  std::vector<int> startPositions;
  std::vector<int> patternLength;
  std::weak_ptr<Node> getMove(std::weak_ptr<Node> node, char symbol);
  std::weak_ptr<Node> makeLink(std::weak_ptr<Node> node);
  std::weak_ptr<Node> makeUP(std::weak_ptr<Node> node);
  void buildTrie();
 public:
  AhoCorasickMachine(const std::string& mask);
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
        ++dividingCharacterNumber;
        patternLength.push_back(currentLength);
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
          patternLength.push_back(currentLength);
          currentLength = 0;
        }
        ++dividingCharacterNumber;
        continue;
      }
      if (hasStared) {
        hasStared = false;
        startPositions.push_back(i);
      }
      ++currentLength;
      dividingCharacterNumber = 0;
      std::shared_ptr<Node> newNode = std::make_shared<Node>(currentNode, mask[i]);
      currentNode->sons.try_emplace(mask[i], newNode);
      currentNode = currentNode->sons[mask[i]];
    }
  }
  if (currentLength > 0) {
    patternLength.push_back(currentLength);
  }
  currentNode->isPatternEnd = true;
  currentNode->patternIndexes.push_back(patternNumber);
}

std::weak_ptr<Node> AhoCorasickMachine::makeLink(std::weak_ptr<Node> node) {
  if (node.lock()->suffixLink.lock() == nullptr) {
    if (node.lock() == trieRoot || node.lock()->parent.lock() == trieRoot) {
      node.lock()->suffixLink = trieRoot;
    } else {
      node.lock()->suffixLink = getMove(makeLink(node.lock()->parent), node.lock()->parentChar);
    }
  }
  return node.lock()->suffixLink;
}

std::weak_ptr<Node> AhoCorasickMachine::getMove(std::weak_ptr<Node> node, char symbol) {
  if (node.lock()->moves.find(symbol) == node.lock()->moves.end()) {
    if (node.lock()->sons.find(symbol) != node.lock()->moves.end()) {
      node.lock()->moves.emplace(symbol, node.lock()->sons[symbol]);
    } else {
      if (node.lock() == trieRoot) {
        node.lock()->moves.emplace(symbol, trieRoot);
      } else {
        node.lock()->moves.emplace(symbol, getMove(makeLink(node), symbol));
      }
    }
  }
  return node.lock()->moves[symbol];
}

std::weak_ptr<Node> AhoCorasickMachine::makeUP(std::weak_ptr<Node> node) {
  if (node.lock()->up.lock() == nullptr) {
    auto currentSuffixLink = makeLink(node);
    if (currentSuffixLink.lock()->isPatternEnd) {
      node.lock()->up = currentSuffixLink;
    } else {
      if (currentSuffixLink.lock() == trieRoot) {
        node.lock()->up = trieRoot;
      } else {
        node.lock()->up = makeUP(currentSuffixLink);
      }
    }
  }
  return node.lock()->up;
}

void AhoCorasickMachine::workForText(std::string& text) {
  std::vector<int> counter(text.length());
  std::weak_ptr<Node> currentNode = trieRoot;
  for (int i = 0; i < text.length(); i++) {
    currentNode = getMove(currentNode, text[i]);
    std::weak_ptr<Node> temporaryNode = currentNode;
    while (temporaryNode.lock() != trieRoot) {
      if (temporaryNode.lock()->isPatternEnd)
        for (auto index: temporaryNode.lock()->patternIndexes)
          if (i - patternLength[index] - startPositions[index] >= -1 && i + dividingCharacterNumber < text.length())
            ++counter[i - patternLength[index] - startPositions[index] + 1];

      temporaryNode = makeUP(temporaryNode);
    }
  }
  for (int i = 0; i < counter.size(); ++i) {
    if (startPositions.empty()) {
      if (i + dividingCharacterNumber <= text.length())
        std::cout << i << " ";
    } else if (counter[i] == startPositions.size())
      std::cout << i << " ";
  }
  std::cout << std::endl;
}
