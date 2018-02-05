#ifndef PROTECTION_SINGLE_H
#define PROTECTION_SINGLE_H

namespace protection {
namespace injections {

template <typename T> class Single {
public:
  static T &instance() {
    static T instance;
    return instance;
  }

  Single(const Single &) = delete;

  Single &operator=(const Single &) = delete;

  Single(Single &&) = delete;

  Single &operator=(Single &&) = delete;

protected:
  Single() = default;

  ~Single() = default;
};

} // namespace injections
} // namespace protection

#endif // PROTECTION_SINGLE_H
