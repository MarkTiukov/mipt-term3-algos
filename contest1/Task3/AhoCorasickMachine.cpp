#include "AhoCorasickMachine.h"
#include <iostream>

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
        patternLength.emplace_back(currentLength);
        currentLength = 0;
      }
      hasStared = true;
      currentNode->isPatternEnd = true;
      currentNode->patternIndexes.emplace_back(patternNumber);
      patternNumber++;
      currentNode = trieRoot;
    } else {
      if (mask[i] == '?') {
        if (currentLength > 0) {
          patternLength.emplace_back(currentLength);
          currentLength = 0;
        }
        ++dividingCharacterNumber;
        continue;
      }
      if (hasStared) {
        hasStared = false;
        startPositions.emplace_back(i);
      }
      ++currentLength;
      dividingCharacterNumber = 0;
      std::shared_ptr<Node> newNode = std::make_shared<Node>(currentNode, mask[i]);
      currentNode->sons.try_emplace(mask[i], newNode);
      currentNode = currentNode->sons[mask[i]];
    }
  }
  if (currentLength > 0) {
    patternLength.emplace_back(currentLength);
  }
  currentNode->isPatternEnd = true;
  currentNode->patternIndexes.emplace_back(patternNumber);
}

std::weak_ptr<AhoCorasickMachine::Node> AhoCorasickMachine::makeLink(const std::weak_ptr<Node>& node) {
  if (node.lock()->suffixLink.lock() == nullptr) {
    if (node.lock() == trieRoot || node.lock()->parent.lock() == trieRoot) {
      node.lock()->suffixLink = trieRoot;
    } else {
      node.lock()->suffixLink = getMove(makeLink(node.lock()->parent), node.lock()->parentChar);
    }
  }
  return node.lock()->suffixLink;
}

std::weak_ptr<AhoCorasickMachine::Node> AhoCorasickMachine::getMove(const std::weak_ptr<Node>& node, char symbol) {
  if (node.lock()->moves.find(symbol) == node.lock()->moves.end()) {
    if (node.lock()->sons.find(symbol) != node.lock()->moves.end()) {
      node.lock()->moves.emplace(symbol, node.lock()->sons[symbol]);
    } else {
      node.lock()->moves.emplace(symbol, node.lock() == trieRoot ? trieRoot : getMove(makeLink(node), symbol));
    }
  }
  return node.lock()->moves[symbol];
}

std::weak_ptr<AhoCorasickMachine::Node> AhoCorasickMachine::makeUP(const std::weak_ptr<Node>& node) {
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

void AhoCorasickMachine::printAllOccurrences(const std::string& text) {
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
  printAnswer(text, counter);
}

void AhoCorasickMachine::printAnswer(const std::string& text, std::vector<int>& counter) {
  for (int i = 0; i < counter.size(); ++i) {
    if (startPositions.empty()) {
      if (i + dividingCharacterNumber <= text.length())
        std::cout << i << " ";
    } else if (counter[i] == startPositions.size())
      std::cout << i << " ";
  }
  std::cout << std::endl;
}
