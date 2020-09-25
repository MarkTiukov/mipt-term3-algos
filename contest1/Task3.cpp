#include <iostream>
#include <string>
#include <vector>
#include <memory>

class AhoCorasickMachine {
 private:
  static const int ALPHABET_SIZE = 26;
  struct Node;
  const std::string mask;
  std::shared_ptr<Node> trieRoot;

  void buildTrie();
 public:
  AhoCorasickMachine(const std::string& mask);
};

struct AhoCorasickMachine::Node {
  std::vector<std::shared_ptr<Node>> sons = std::vector<std::shared_ptr<Node>>(ALPHABET_SIZE); /// сыновья/исходящие связи
  std::vector<std::shared_ptr<Node>> fastMove = std::vector<std::shared_ptr<Node>>(ALPHABET_SIZE); /// переходы автомата
  std::shared_ptr<Node> suffixLink; /// суффиксная ссылка
  std::shared_ptr<Node> up; /// сжатая ссылка
  std::shared_ptr<Node> parent; /// ссылка на родителя
  char parentChar; /// символ на переходе из родителя в текущую вершину
  bool isPatternEnd; /// является ли концом некоторого шаблона
  std::vector<int> patternIndexes; ///  шаблоны, с которыми связана данная вершина

  Node(const std::shared_ptr<Node>& parent, char parent_char) : parent(parent), parentChar(parent_char) {}

};

AhoCorasickMachine::AhoCorasickMachine(const std::string& mask) : mask(mask) {}

void AhoCorasickMachine::buildTrie() {
  std::shared_ptr<Node> currentNode = trieRoot;
  for (int i = 0; i < mask.length(); ++i) {
    if (currentNode != trieRoot && mask[i] == '?') {
      currentNode->isPatternEnd = true;
      currentNode = trieRoot;
    } else {
      bool found = false;
      for (auto son: currentNode->sons) {
        if (!found && son->parentChar == mask[i]) {
          currentNode = son;
          found = true;
        }
      }
      if (!found) {
        currentNode->sons.emplace_back(std::make_shared<Node>(currentNode, mask[i]));
      }
    }
  }
}

int main() {

}
