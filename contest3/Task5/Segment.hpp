#ifndef Segment_hpp
#define Segment_hpp

#include <iostream>

#include "Point.hpp"
#include "Vector.hpp"

static const double PRECISION = 1e-6;

class Segment {
private:
    Point begin, end;
public:
    const size_t id;
    
    Segment(size_t id) : id(id) {}
    Segment(const Point& begin, const Point& end, size_t id) : begin(begin), end(end), id(id) {}
    
    double getY(double x) const;
    long long getMin() const;
    long long getMax() const;
    bool hasIntersectionByX(const Segment& other) const;
    bool hasIntersectionByY(const Segment& other) const;
    bool hasIntersectionWith(const Segment& other) const;
    
    bool operator<(const Segment& other) const;
    
    friend std::istream& operator>>(std::istream& input, Segment& segment);
    friend std::ostream& operator<<(std::ostream& output, const Segment& segment);
};

#endif
