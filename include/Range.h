#pragma once

#include <functional>
#include <stdint.h>
#include <string>

#ifdef _MSC_VER
#ifdef LIBPRROTECTION_DLL
#define LIBPRROTECTION_EXPORT __declspec(dllexport)
#else
#define LIBPRROTECTION_EXPORT __declspec(dllimport)
#endif
#else
#define LIBPRROTECTION_EXPORT /**/
#endif

/// Simple [begin, end] range.
struct LIBPRROTECTION_EXPORT Range {
  size_t lowerBound;
  size_t upperBound;
  Range(size_t lBound, size_t uBound);
  bool contains(size_t point) const;
  bool contains(const Range &range) const;
  bool overlaps(const Range &range) const;
  std::string toString() const;
  bool operator==(const Range &range) const;
};

namespace std {
template <> struct hash<Range> {
  size_t operator()(const Range &r) const {
    auto hashCode = r.lowerBound;
    hashCode = (hashCode * 397) ^ r.upperBound;
    return hashCode;
  }
};
} // namespace std
