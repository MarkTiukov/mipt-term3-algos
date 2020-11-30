#ifndef IntersectionFinder_hpp
#define IntersectionFinder_hpp

#include <set>
#include "Segment.hpp"


class IntersectionFinder {
private:
    std::set<Segment> currentSegments;
    
    std::set<Segment>::iterator prev(std::set<Segment>::iterator it);
    std::set<Segment>::iterator next(std::set<Segment>::iterator it);
public:
    IntersectionFinder() = default;
    std::pair<bool, std::pair<Segment, Segment>> findAnyIntersection(const std::vector<Segment>& segments);
};

#endif
