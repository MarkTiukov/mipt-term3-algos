#include "geometry.h"

bool operator==(const Point& one, const Point &other) {
    return one.x == other.x && one.y == other.y;
}

bool operator>(const Point& one, const Point &other) {
    return !(one < other || one == other);
}

bool operator!=(const Point& one, const Point &other) {
    return !(one == other);
}

bool operator<(const Point& one, const Point& another) {
    bool result = one.y < another.y;
    if (one.y == another.y)
        result = one.x < another.x;
    return result;
}

std::ostream& operator<<(std::ostream& outputStream, const Point& point) {
    outputStream << point.x << " " << point.y;
    return outputStream;
}

long long Vector::zCoordinateOfCrossProduct(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}


Point& Point::operator-=(const Point& another) {
    this->x -= another.x;
    this->y -= another.y;
    return *this;
}

Point operator-(const Point& one, const Point& another) {
    Point result = one;
    result -= another;
    return result;
}

bool Vector::operator==(const Vector& another) const {
    return this->x == another.x && this->y == another.y;
}

bool Vector::operator!=(const Vector& another) const {
    return !(*this == another);
}
