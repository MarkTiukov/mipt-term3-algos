#include <string>
#include <vector>

class PatternMatching {
 private:
  std::string data;
  std::string pattern;
  int dataLength;
  int patternLength;
  inline static const std::string DEFAULT_PATTERN_SEPARATOR = "#";
  std::vector<int> prefixFunction;
  std::vector<int> patternPositions;

 public:

  PatternMatching() = default;
  PatternMatching(const std::string& data, const std::string& pattern);

  const std::vector<int>& getPatternPositions() const;

  void findPattern();
  void countPrefixFunction(std::string&& string, std::vector<int>& answer);

  virtual ~PatternMatching() = default;
};