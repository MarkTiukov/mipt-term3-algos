#include "Polygon.hpp"

std::istream& operator>>(std::istream& in, Polygon& polygon) {
    size_t number = 0;
    std::cin >> number;
    for (int i = 0; i < number; ++i) {
        Point newPoint;
        std::cin >> newPoint.x >> newPoint.y;
        polygon += newPoint;
    }
    return in;
}

Polygon& Polygon::operator+=(const Point& newPoint) {
    this->points.emplace_back(newPoint);
    return *this;
}

Polygon Polygon::operator+(const Point& newPoint) const {
    Polygon result = *this;
    result += newPoint;
    return result;
}

Polygon& Polygon::operator+=(const Polygon& other) {
    size_t n = points.size();
    size_t m = other.points.size();
    Polygon otherCopy = other;
    *this += points[0];
    otherCopy += other.points[0];
    std::vector<Point> result;
    size_t i = 0, j = 0;
    while (i < n && j < m) {
        result.emplace_back(points[i] + otherCopy.points[j]);
        Vector v1(points[i], points[i + 1]), v2(otherCopy.points[j], otherCopy.points[j + 1]);
        if (Vector::areCollinear(v1, v2)) {
            ++i;
            ++j;
        } else if (compareByAngle(v1, v2)) {
            ++i;
        } else {
            ++j;
        }
    }
    while (i < n) {
        result.emplace_back(points[i] + otherCopy.points[j]);
        ++i;
    }
    while (j < m) {
        result.emplace_back(points[i] + otherCopy.points[j]);
        ++j;
    }
    this->points = result;
    return *this;
}

Polygon Polygon::operator+(const Polygon& other) const {
    Polygon result = *this;
    result += other;
    return result;
}

long double Polygon::getSquare() const {
    // TODO: add counting the square
    size_t n = points.size();
    long double result = (points[n - 1].x - points[0].x) * (points[0].y + points[n - 1].y);
    for (size_t i = 0; i < points.size() - 1; ++i) {
        result += (points[i].x - points[i + 1].x) * (points[i + 1].y + points[i].y);
    }
    return result / 2;
}

bool Polygon::isLowerAndToTheLeft(const Point& p1, const Point& p2) {
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

bool Polygon::compareByAngle(const Point& p1, const Point& p2, const Point& begin) {
    Vector v1(p1 - begin);
    Vector v2(p2 - begin);
    return compareByAngle(v1, v2);
}

bool Polygon::compareByAngle(const Vector &v1, const Vector &v2) {
    long long product = Vector::crossProduct(v1, v2);
    if (product == 0) {
        if (v1 == Vector(0, 0))
            return true;
        if (v2 == Vector(0, 0))
            return false;
        return v1.getX() * v1.getX() + v1.getY() * v1.getY() > v2.getX() * v2.getX() + v2.getY() * v2.getY();
    }
    return product > 0;
}

void Polygon::sort() {
    std::sort(points.begin(), points.end(), LowerAndToTheLeftComparator(this));
    std::sort(++points.begin(), points.end(), AngleComparator(this, points[0]));
}
