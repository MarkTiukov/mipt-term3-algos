#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

static const long double PRECISION = 1e-6;

class Point {
public:
    long long x, y;
    
    Point() : x(0), y(0) {}
    Point(long long x, long long y) : x(x), y(y) {}
    
    Point& operator+=(const Point& other);
    Point operator+(const Point& other) const;
    Point& operator-=(const Point& other);
    Point operator-(const Point& other) const;
    
};

struct Vector {
    Point begin;
    Point end;
    long long x, y;
    
    Vector() = default;
    Vector(const Point& begin, const Point& end) : begin(begin), end(end),
    x(end.x - begin.x),
    y(end.y - begin.y) {}
    Vector(const Point& radiusVector) : end(radiusVector),
    x(radiusVector.x),
    y(radiusVector.y) {}
    Vector(long long x, long long y) : x(x), y(y), end(Point(x, y)) {}
    
    static long long crossProduct(const Vector& v1, const Vector& v2);
    static bool areCollinear(const Vector& v1, const Vector& v2);
    
    bool operator==(const Vector& another) const;
    bool operator!=(const Vector& another) const;
    
};

class Polygon {
private:
    std::vector<Point> points;
    
    static bool isLowerAndToTheLeft(const Point& p1, const Point& p2);
    static bool compareByAngle(const Point& p1, const Point& p2, const Point& begin);
    static bool compareByAngle(const Vector& v1, const Vector& v2);

    
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
    
    void sort();
    long double getSquare() const;
};

std::istream& operator>>(std::istream& in, Polygon& polygon);

int main() {
    Polygon A, B;
    std::cin >> A >> B;
    A.sort();
    B.sort();
    Polygon sum = A + B;
    std::cout << std::fixed << std::setprecision(6) << (sum.getSquare() - A.getSquare() - B.getSquare()) / 2 << std::endl;
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
        return v1.x * v1.x + v1.y * v1.y > v2.x * v2.x + v2.y * v2.y;
    }
    return product > 0;
}

void Polygon::sort() {
    std::sort(points.begin(), points.end(), LowerAndToTheLeftComparator(this));
    std::sort(++points.begin(), points.end(), AngleComparator(this, points[0]));
}



bool Vector::operator==(const Vector& another) const {
    return this->x == another.x && this->y == another.y;
}

bool Vector::operator!=(const Vector& another) const {
    return !(*this == another);
}

long long Vector::crossProduct(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

bool Vector::areCollinear(const Vector &v1, const Vector &v2) {
    return Vector::crossProduct(v1, v2) == 0;
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

Point& Point::operator+=(const Point& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Point Point::operator+(const Point& other) const {
    Point result = *this;
    result += other;
    return result;
}
