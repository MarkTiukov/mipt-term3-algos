#include <iostream>
#include <vector>
#include <algorithm>

class Point {
public:
    long long x, y;
    
    Point() : x(0), y(0) {}
    Point(long long x, long long y) : x(x), y(y) {}
    
    Point& operator-=(const Point& other);
    Point operator-(const Point& other) const;
    
};

struct MyVector {
    Point begin;
    Point end;
    long long x, y;
    
    MyVector() = default;
    MyVector(const Point& begin, const Point& end) : begin(begin), end(end),
    x(begin.x - end.x),
    y(begin.y - end.y) {}
    MyVector(const Point& radiusVector) : end(radiusVector),
    x(radiusVector.x),
    y(radiusVector.y) {}
    MyVector(long long x, long long y) : x(x), y(y), end(Point(x, y)) {}
    
    static long long crossProduct(const MyVector& v1, const MyVector& v2);
    
    bool operator==(const MyVector& another);
    bool operator!=(const MyVector& another);
    
};

class Polygon {
private:
    std::vector<Point> points;
    
    static bool isLowerAndToTheLeft(const Point& p1, const Point& p2);
    static bool compareByAngle(const Point& p1, const Point& p2, const Point& begin);
    
    struct LowerAndToTheLeftComparator : std::binary_function<Point, Point, bool> {
        Polygon *polygon;
        
        LowerAndToTheLeftComparator(Polygon *p) : polygon(p) {}
        bool operator()(const Point& p1, const Point& p2) {
            return polygon->isLowerAndToTheLeft(p1, p2);
        }
    };
    
    struct AngleComparator : std::binary_function<Point, Point, bool> {
        Polygon *polygon;
        Point begin;
        AngleComparator(Polygon *p, const Point& center) : polygon(p), begin(center) {}
        bool operator()(const Point& p1, const Point& p2) {
            return polygon->compareByAngle(p1, p2, this->begin);
        }
    };
    
public:
    Polygon() = default;
    Polygon(std::vector<Point> points) : points(points) {}
    
    Polygon& operator=(const Polygon& other) = default;
    Polygon& operator+=(const Polygon& other);
    Polygon operator+(const Polygon& other) const;
    
    Polygon& operator+=(const Point& newPoint);
    Polygon operator+(const Point& newPoint) const;
    
    double getSquare();
};

std::istream& operator>>(std::istream& in, Polygon& polygon);

int main() {
    Polygon A, B;
    std::cin >> A >> B;
    std::cout << "started:" << std::endl;
    Polygon sum = A + B;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    this->points.push_back(newPoint);
    return *this;
}

Polygon Polygon::operator+(const Point& newPoint) const {
    Polygon result = *this;
    result += newPoint;
    return result;
}

Polygon& Polygon::operator+=(const Polygon& other) {
    // TODO: finish the realization of Minkowski addition
    std::sort(points.begin(), points.end(), LowerAndToTheLeftComparator(this));
    std::sort(++points.begin(), points.end(), AngleComparator(this, points[0]));
    Polygon otherCopy = other;
    std::sort(otherCopy.points.begin(), otherCopy.points.end(), LowerAndToTheLeftComparator(this));
    std::sort(++otherCopy.points.begin(), otherCopy.points.end(), AngleComparator(this, otherCopy.points[0]));

    *this += points[0];
    otherCopy += other.points[0];
    return *this;
}

Polygon Polygon::operator+(const Polygon& other) const {
    Polygon result = *this;
    result += other;
    return result;
}

double Polygon::getSquare() {
    // TODO: add counting the square
    double result = 0;
    return result;
}

bool Polygon::isLowerAndToTheLeft(const Point& p1, const Point& p2) {
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

bool Polygon::compareByAngle(const Point& p1, const Point& p2, const Point& begin) {
    MyVector v1(p1 - begin);
    MyVector v2(p2 - begin);
    long long product = MyVector::crossProduct(v1, v2);
    if (product == 0) {
        if (v1 == MyVector(0, 0))
            return true;
        if (v2 == MyVector(0, 0))
            return false;
        return v1.x * v1.x + v1.y * v1.y > v2.x * v2.x + v2.y * v2.y;
    }
    return product > 0;
}

bool MyVector::operator==(const MyVector& another) {
    return this->x == another.x && this->y == another.y;
}

bool MyVector::operator!=(const MyVector& another) {
    return !(*this == another);
}

long long MyVector::crossProduct(const MyVector& v1, const MyVector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

Point& Point::operator-=(const Point& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Point Point::operator-(const Point& other) const {
    Point result = *this;
    result -= other;
    return result;
}
