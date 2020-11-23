#include <vector>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iostream>


struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
    Point& operator-=(const Point& another);
    friend bool operator<(const Point& one, const Point& another);
    friend bool operator>(const Point& one, const Point& another);
    friend bool operator==(const Point& one, const Point& another);
    friend bool operator!=(const Point& one, const Point& another);
    friend Point operator-(const Point& one, const Point& another);
    friend std::ostream& operator<<(std::ostream& outputStream, const Point& point);

};

class Vector {
private:
    Point begin;
    Point end;
    long long x, y;
public:
    
    Vector() = default;
    Vector(const Point& begin, const Point& end) : begin(begin), end(end),
    x(end.x - begin.x),
    y(end.y - begin.y) {}
    Vector(const Point& radiusVector) : end(radiusVector),
    x(radiusVector.x),
    y(radiusVector.y) {}
    Vector(long long x, long long y) : x(x), y(y), end(Point(x, y)) {}
    
    long long getX() const { return x; }
    long long getY() const { return y; }
    Point getBegin() const { return begin; }
    Point getEnd() const { return end; }
    
    bool operator==(const Vector& another) const;
    bool operator!=(const Vector& another) const;
    
    static long long zCoordinateOfCrossProduct(const Vector& v1, const Vector& v2);
    
};
