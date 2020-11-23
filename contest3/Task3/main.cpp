#include <iostream>
#include <vector>
#include <iomanip>

#include "Point.hpp"
#include "Vector.hpp"
#include "Face.hpp"
#include "ConvexHull.hpp"

const int OUTPUT_PRECISION = 5;

int main() {
    size_t n;
    std::cin >> n;
    std::vector<Point> points;
    for (size_t i = 0; i < n; ++i) {
        Point point;
        std::cin >> point;
        points.push_back(point);
    }
    ConvexHull convexHull(points);
    size_t q;
    std::cin >> q;
    for (int i = 0; i < q; ++i) {
        Point point;
        std::cin >> point;
        std::cout << std::setprecision(OUTPUT_PRECISION) << convexHull.distanceToInsidePoint(point) << "\n";
    }
}

