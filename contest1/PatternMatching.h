#include <string>
#include <vector>

class PatternMatching {
 private:
  std::string data;
  std::string pattern;
  int dataLength;
  int patternLength;
  static const char DEFAULT_PATTERN_SEPARATOR = '#';
  std::vector<int> prefixFunction;
  std::vector<int> patternPositions;

 public:

  PatternMatching() = default;
  PatternMatching(const std::string& data, const std::string& pattern);

  void findPattern();
  void printPatternPositions();

  void countPrefixFunction(std::string&& string, std::vector<int>& answer);

  virtual ~PatternMatching() = default;
};