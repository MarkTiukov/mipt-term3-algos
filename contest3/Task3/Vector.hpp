#ifndef Vector_hpp
#define Vector_hpp

#include "Point.hpp"

class Vector {
private:
    Point begin;
    Point end;
    double x, y, z;
public:
    
    Vector() = default;
    Vector(const Point& begin, const Point& end) : begin(begin), end(end),
    x(end.x - begin.x),
    y(end.y - begin.y),
    z(end.z - begin.z){}
    Vector(double x, double y, double z) : x(x), y(y), z(z), end(Point(x, y, z)) {}
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    Point getBegin() const { return begin; }
    Point getEnd() const { return end; }
    
    friend Vector operator*(const Vector& one, const Vector& other);
};

#endif
