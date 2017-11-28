#include "Range.h"

#include <sstream>

Range::Range(size_t lBound, size_t uBound) : lowerBound{lBound}, upperBound{uBound} {}

bool Range::contains(size_t point) const { return lowerBound <= point && upperBound >= point; }

bool Range::contains(const Range &range) const { return contains(range.lowerBound) && contains(range.upperBound); }

bool Range::overlaps(const Range &range) const {
  return (lowerBound >= range.lowerBound && upperBound <= range.upperBound) || contains(range.lowerBound) ||
         contains(range.upperBound);
}

std::string Range::toString() const {
  std::stringstream ss;
  ss << lowerBound << ".." << upperBound;
  return ss.str();
}

bool Range::operator==(const Range &range) const {
  return (lowerBound == range.lowerBound) && (upperBound == range.upperBound);
}
