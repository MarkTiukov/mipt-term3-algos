#include "Vector.hpp"

long long Vector::zCoordinateOfCrossProduct(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

long long Vector::zCoordinateOfCrossProduct(const Point &begin, const Point &end1, const Point &end2) {
    return zCoordinateOfCrossProduct(Vector(end1 - begin), Vector(end2 - begin));
}
