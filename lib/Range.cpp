#include "protection/Range.h"

#include <cassert>
#include <sstream>

Range::Range(size_t lBound, size_t uBound) : lowerBound{lBound}, upperBound{uBound} {
  assert(lowerBound <= upperBound);
}

size_t Range::length() const { return upperBound - lowerBound; }

bool Range::contains(size_t point) const { return lowerBound <= point && upperBound > point; }

bool Range::contains(const Range &range) const { return contains(range.lowerBound) && contains(range.upperBound); }

bool Range::overlaps(const Range &range) const {
  return (lowerBound >= range.lowerBound && upperBound <= range.upperBound) || contains(range.lowerBound) ||
         contains(range.upperBound - 1);
}

std::string Range::toString() const {
  std::stringstream ss;
  (length() != 0) ? ss << "[" << lowerBound << ".." << upperBound << ")" : ss << "[" << lowerBound << ")";
  return ss.str();
}

bool Range::operator==(const Range &range) const {
  return (lowerBound == range.lowerBound) && (upperBound == range.upperBound);
}
