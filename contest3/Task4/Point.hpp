#ifndef Point_hpp
#define Point_hpp

#include <iostream>

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


#endif
