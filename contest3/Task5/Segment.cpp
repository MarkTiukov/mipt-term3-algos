#include "Segment.hpp"

double Segment::getY(double x) const {
    if (begin.x == end.x)
        return begin.y;
    return begin.y + (end.y - begin.y) * (x - begin.x) / (end.x - begin.x);
}

long long Segment::getMin() const {
    return std::min(begin.x, end.x);
}

long long Segment::getMax() const {
    return std::max(begin.x, end.x);
}

std::istream& operator>>(std::istream& input, Segment& segment) {
    std::cin >> segment.begin >> segment.end;
    return input;
}

std::ostream& operator<<(std::ostream& output, const Segment& segment) {
    std::cout << segment.begin << " " << segment.end << std::endl;
    return output;
}

bool Segment::operator<(const Segment& other) const {
    double x = std::max(std::min(begin.x, end.x), std::min(other.begin.x, other.end.x));
    return getY(x) < other.getY(x) - PRECISION;
}

bool Segment::hasIntersectionWith(const Segment &other) const {
    bool result = false;
    if (hasIntersectionByX(other) && hasIntersectionByY(other))
        if (Vector::zCoordinateOfCrossProduct(begin, end, other.begin) *
            Vector::zCoordinateOfCrossProduct(begin, end, other.end) <= 0 &&
            Vector::zCoordinateOfCrossProduct(other.begin, other.end, begin) *
            Vector::zCoordinateOfCrossProduct(other.begin, other.end, end) <= 0)
            result = true;
    return result;
}

bool Segment::hasIntersectionByX(const Segment &other) const {
    long long leftEnd1 = std::min(begin.x, end.x);
    long long rightEnd1 = std::max(begin.x, end.x);
    long long leftEnd2 = std::min(other.begin.x, other.end.x);
    long long rightEnd2 = std::max(other.begin.x, other.end.x);
    return std::max(leftEnd1, leftEnd2) <= std::min(rightEnd1, rightEnd2);
}

bool Segment::hasIntersectionByY(const Segment &other) const {
    long long lowerEnd1 = std::min(begin.y, end.y);
    long long upperEnd1 = std::max(begin.y, end.y);
    long long lowerEnd2 = std::min(other.begin.y, other.end.y);
    long long upperEnd2 = std::max(other.begin.y, other.end.y);
    return std::max(lowerEnd1, lowerEnd2) <= std::min(upperEnd1, upperEnd2);
}
