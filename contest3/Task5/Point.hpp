#ifndef Point_hpp
#define Point_hpp

#include <iostream>

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    Point& operator-=(const Point& other);
    friend Point operator-(const Point& one, const Point& other);
    
    friend std::istream& operator>>(std::istream& input, Point& point);
    friend std::ostream& operator<<(std::ostream& output, const Point& segment);
};

#endif
