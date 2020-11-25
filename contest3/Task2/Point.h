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

