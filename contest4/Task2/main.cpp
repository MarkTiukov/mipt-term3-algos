#include <iostream>
#include <vector>
#include <unordered_set>

class CruelGameSolver {
private:
    
    void initializeNumbers(std::vector<int> &answers) const;
    int mex(const std::unordered_set<int> &set) const;
    std::vector<int> findFirstPossibleMoves(const std::vector<int> &answers) const;
    
public:
    CruelGameSolver() = default;
    
    std::pair<int, std::vector<int>> solve(int n);
};

int main() {
    int n;
    std::cin >> n;
    CruelGameSolver solver;
    auto result = solver.solve(n);
    std::cout << (result.first == 0 ? "Mueller" : "Schtirlitz") << std::endl;
    if (result.first != 0)
        for (auto possibleFirstMove : result.second)
            std::cout << possibleFirstMove << std::endl;
}

std::pair<int, std::vector<int>> CruelGameSolver::solve(int n) {
    std::vector<int> answers(n + 1);
    initializeNumbers(answers);
    for (int i = 4; i <= n; ++i) {
        std::unordered_set<int> resultsForConnectedGames;
        resultsForConnectedGames.emplace(answers[i - 1]);
        resultsForConnectedGames.emplace(answers[i - 2]);
        for (int j = 2; j < i - 2; ++j)
            resultsForConnectedGames.emplace(answers[j] ^ answers[i - j - 1]);
        answers[i] = mex(resultsForConnectedGames);
    }
    return std::make_pair(answers[n], findFirstPossibleMoves(answers));
}

void CruelGameSolver::initializeNumbers(std::vector<int> &answers) const {
    answers[0] = 0;
    if (answers.size() > 1)
        answers[1] = 1;
    if (answers.size() > 2)
        answers[2] = 1;
    if (answers.size() > 3)
        answers[3] = 2;
    
}

int CruelGameSolver::mex(const std::unordered_set<int> &set) const {
    int mex = 0;
    while (set.find(mex) != set.end())
        ++mex;
    return mex;
}

std::vector<int> CruelGameSolver::findFirstPossibleMoves(const std::vector<int> &answers) const {
    size_t n = answers.size() - 1;
    std::vector<int> firstPossibleMoves;
    switch (n) {
        case 1:
            firstPossibleMoves = {1};
            break;
        case 2:
            firstPossibleMoves = {1, 2};
            break;
        case 3:
            firstPossibleMoves = {2};
            break;
        default:
            if (answers[n - 1] == 0)
                firstPossibleMoves.push_back(1);
            if (answers[n - 2] == 0)
                firstPossibleMoves.push_back(2);
            for (int i = 2; i < n - 2; ++i) 
                if ((answers[i] ^  answers[n - i - 1]) == 0)
                    firstPossibleMoves.push_back(i + 1);
            if (answers[n - 2] == 0)
                firstPossibleMoves.push_back(n - 1);
            if (answers[n - 1] == 0)
                firstPossibleMoves.push_back(n);
            break;
    }
    return firstPossibleMoves;
}



