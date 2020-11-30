#include "IntersectionFinder.hpp"

#include <vector>
#include "Segment.hpp"
#include "Event.hpp"

std::pair<bool, std::pair<Segment, Segment>> IntersectionFinder::findAnyIntersection(const std::vector<Segment>& segments) {
    currentSegments.clear();
    std::vector<std::set<Segment>::iterator> segmentsInSet(segments.size());
    std::vector<Event> events;
    for (size_t i = 0; i < segments.size(); ++i) {
        events.emplace_back(segments[i].getMin(), EventType::ADD, i);
        events.emplace_back(segments[i].getMax(), EventType::REMOVE, i);
    }
    std::sort(events.begin(), events.end());
    for (auto event : events) {
        size_t currentID = event.getID();
        if (event.getType() == EventType::ADD) {
            std::set<Segment>::iterator nextSegmentIterator = currentSegments.lower_bound(segments[currentID]);
            std::set<Segment>::iterator prevSegmentIterator = prev(nextSegmentIterator);
            if (nextSegmentIterator != currentSegments.end() && (*nextSegmentIterator).hasIntersectionWith(segments[currentID]))
                return std::make_pair(true, std::make_pair(*nextSegmentIterator, segments[currentID]));
            if (prevSegmentIterator != currentSegments.end() && (*prevSegmentIterator).hasIntersectionWith(segments[currentID]))
                return std::make_pair(true, std::make_pair(*prevSegmentIterator, segments[currentID]));
            segmentsInSet[currentID] = currentSegments.insert(nextSegmentIterator, segments[currentID]);
        } else if (event.getType() == EventType::REMOVE) {
            std::set<Segment>::iterator nextSegmentIterator = next(segmentsInSet[currentID]);
            std::set<Segment>::iterator prevSegmentIterator = prev(segmentsInSet[currentID]);
            if (nextSegmentIterator != currentSegments.end() && prevSegmentIterator != currentSegments.end() &&
                (*prevSegmentIterator).hasIntersectionWith(*nextSegmentIterator))
                return std::make_pair(true, std::make_pair(*prevSegmentIterator, *nextSegmentIterator));
            currentSegments.erase(segmentsInSet[currentID]);
        }
    }
    return std::make_pair(false, std::make_pair(Segment(0), Segment(0)));
}

std::set<Segment>::iterator IntersectionFinder::prev(std::set<Segment>::iterator it) {
    if (it != currentSegments.begin())
        return std::prev(it);
    return currentSegments.end();
}

std::set<Segment>::iterator IntersectionFinder::next(std::set<Segment>::iterator it) {
    return std::next(it);
}
