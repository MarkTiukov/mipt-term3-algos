#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "Point.hpp"
#include "Vector.hpp"
#include "Polygon.hpp"

static const int OUTPUT_PRECISION = 6;

int main() {
    Polygon A, B;
    std::cin >> A >> B;
    A.sort();
    B.sort();
    Polygon sum = A + B;
    std::cout << std::fixed << std::setprecision(OUTPUT_PRECISION) << (sum.getSquare() - A.getSquare() - B.getSquare()) / 2 << std::endl;
}
