#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#include "Point.hpp"
#include "Vector.hpp"
#include "Segment.hpp"
#include "Event.hpp"
#include "IntersectionFinder.hpp"

static const int numerationShift = 1;

std::vector<Segment> readSegments();

void printResult(std::pair<Segment, Segment> result);
void printResult();

int main() {
    auto segments = readSegments();
    IntersectionFinder intersectionFinder;
    auto answer = intersectionFinder.findAnyIntersection(segments);
    if (answer.first)
        printResult(answer.second);
    else
        printResult();
}

std::vector<Segment> readSegments() {
    size_t n;
    std::cin >> n;
    std::vector<Segment> segments;
    for (size_t i = 0; i < n; ++i) {
        segments.emplace_back(i);
        std::cin >> segments[i];
    }
    return segments;
}

void printResult(std::pair<Segment, Segment> result) {
    std::cout << "YES\n" << result.first.id + numerationShift << " " << result.second.id + numerationShift << std::endl;
}

void printResult() {
    std::cout << "NO" << std::endl;
}
