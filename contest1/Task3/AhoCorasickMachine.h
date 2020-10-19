#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class AhoCorasickMachine {
 private:
  static const int ALPHABET_SIZE = 26;

  struct Node {
    std::unordered_map<char, std::shared_ptr<Node>>
        sons = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// сыновья / исходящие связи
    std::unordered_map<char, std::shared_ptr<Node>> moves = std::unordered_map<char, std::shared_ptr<Node>>(ALPHABET_SIZE); /// переходы автомата
    std::weak_ptr<Node> suffixLink = std::shared_ptr<Node>(nullptr); /// суффиксная ссылка
    std::weak_ptr<Node> up = std::shared_ptr<Node>(nullptr); /// сжатая ссылка
    std::weak_ptr<Node> parent = std::shared_ptr<Node>(nullptr); /// ссылка на родителя
    char parentChar; /// символ на переходе из родителя в текущую вершину
    bool isPatternEnd = false; /// является ли концом некоторого шаблона
    std::vector<int> patternIndexes; ///  шаблоны, с которыми связана данная вершина

    Node() : parentChar(0) {}
    Node(std::weak_ptr<Node> parent, char parentChar) : parent(std::move(parent)), parentChar(parentChar) {}
  };

  const std::string mask;
  int dividingCharacterNumber = 0;
  std::shared_ptr<Node> trieRoot;
  std::vector<int> startPositions;
  std::vector<int> patternLength;
  std::weak_ptr<Node> getMove(const std::weak_ptr<Node>& node, char symbol);
  std::weak_ptr<Node> makeLink(const std::weak_ptr<Node>& node);
  std::weak_ptr<Node> makeUP(const std::weak_ptr<Node>& node);
  void buildTrie();
  void printAnswer(const std::string& text, std::vector<int>& counter);

 public:
  explicit AhoCorasickMachine(const std::string& mask);
  void printAllOccurrences(const std::string& text);
};
