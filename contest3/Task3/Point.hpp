#ifndef Point_hpp
#define Point_hpp

#include <iostream>

struct Point {
    double x, y, z;
    
    Point() = default;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    Point& operator-=(const Point& other);
    friend Point operator-(const Point& one, const Point& other);
    friend std::istream& operator>>(std::istream& input, Point& point);
};

#endif /* Point_hpp */
