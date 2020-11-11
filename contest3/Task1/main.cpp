#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>


const double fPRECISION = std::numeric_limits<float>::epsilon();

class Point {
public:
    double x, y, z;
    Point() = default;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point operator+(const Point &point) const;
    Point& operator+=(const Point &point);
    bool operator==(const Point &point) const;
    bool operator!=(const Point &point) const;
    Point& operator*=(const double number);
    Point operator*(const double number) const;
    Point& operator/=(const double number);
    Point operator/(const double number) const;
};

class Segment {
public:
    Point begin, end;
    
    Segment() = default;
    Segment(Point begin, Point end) : begin(begin), end(end) {}

    double getLengthSquared();
    double getLength();
    
};

double getHeightFromPointOnSegment(const Segment &segment, const Point &point);

bool checkIntersectionOnProjection(const Segment &segment1, const Segment &segment2);
bool checkIntersection(const Segment &segment1, const Segment &segment2);

double findMinDistanceBetweenTwoSegments(const Segment &segment1, const Segment &segment2);

int main() {
    Point a, b, c, d;
    std::cin >> a.x >> a.y >> a.z;
    std::cin >> b.x >> b.y >> b.z;
    std::cin >> c.x >> c.y >> c.z;
    std::cin >> d.x >> d.y >> d.z;
    double result = findMinDistanceBetweenTwoSegments(Segment(a, b), Segment(c, d));
    std::cout << std::setprecision(9) << (std::abs(result) < fPRECISION ? 0 : result) << std::endl;
}

double findMinDistanceBetweenTwoSegments(const Segment &segment1, const Segment &segment2) {
    Point begin = segment1.begin;
    Point end = segment2.begin;
    Point new_begin = segment1.begin;
    Point new_end = segment2.begin;
    Point left, right;
    for (int stepNumber = 0; stepNumber < 20000; ++stepNumber) {
        begin = new_begin;
        end   = new_end;
        left  = segment1.begin;
        right = segment1.end;
        while (left != right) {
            Point a = (left * 2 + right) / 3;
            Point b = (left + right * 2) / 3;
            if (Segment(a, end).getLength() <
                Segment(b, end).getLength()) {
                right = b;
            } else {
                left = a;
            }
        }
        new_begin = left;
        left  = segment2.begin;
        right = segment2.end;
        while (left != right) {
            auto a = (left * 2 + right) / 3;
            auto b = (left + right * 2) / 3;
            if (Segment(new_begin, a).getLength() <
                Segment(new_begin, b).getLength()) {
                right = b;
            } else {
                left = a;
            }
        }
        new_end = left;
    }
    
    return Segment(begin, end).getLength();
}

Point& Point::operator+=(const Point &point) {
    this->x += point.x;
    this->y += point.y;
    this->z += point.z;
    return *this;
}

Point Point::operator+(const Point &point) const {
    Point result = *this;
    result += point;
    return result;
}

bool Point::operator==(const Point &point) const {
    return std::abs(this->x - point.x) < fPRECISION &&
            std::abs(this->y - point.y) < fPRECISION &&
            std::abs(this->z - point.z) < fPRECISION;
}

bool Point::operator!=(const Point &point) const {
    return !(*this == point);
}

Point& Point::operator*=(const double number) {
    this->x *= number;
    this->y *= number;
    this->z *= number;
    return *this;
}

Point Point::operator*(const double number) const {
    Point result = *this;
    result *= number;
    return result;
}

Point& Point::operator/=(const double number) {
    this->x /= number;
    this->y /= number;
    this->z /= number;
    return *this;
}

Point Point::operator/(const double number) const {
    Point result = *this;
    result /= number;
    return result;
}

double Segment::getLength() {
    return std::sqrt(getLengthSquared());
}

double Segment::getLengthSquared() {
    return (begin.x - end.x) * (begin.x - end.x) +
            (begin.y - end.y) * (begin.y - end.y) +
            (begin.z - end.z) * (begin.z - end.z);
}
