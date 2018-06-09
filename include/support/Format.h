#ifndef PROTECTION_FORMAT_H
#define PROTECTION_FORMAT_H

#include "protection/Range.h"

#include <iostream>

#include <algorithm>
#include <fmt/format.h>

// using namespace fmt;

namespace protection {
namespace injections {

template <typename Char> class ArgFormatter;

template <typename CharType, typename ArgFormatter = ArgFormatter<CharType>> class BasicFormatter;

template <typename Impl, typename Char, typename Spec = fmt::FormatSpec>
class BasicArgFormatter : public fmt::internal::ArgFormatterBase<Impl, Char, Spec> {
private:
  BasicFormatter<Char, Impl> &formatter_;
  const Char *format_;

public:
  /**
    \rst
    Constructs an argument formatter object.
    *formatter* is a reference to the main formatter object, *spec* contains
    format specifier information for standard argument types, and *fmt* points
    to the part of the format string being parsed for custom argument types.
    \endrst
   */
  BasicArgFormatter(BasicFormatter<Char, Impl> &formatter, Spec &spec, const Char *fmt)
      : fmt::internal::ArgFormatterBase<Impl, Char, Spec>(formatter.writer(), spec), formatter_(formatter),
        format_(fmt) {}

  /** Formats an argument of a custom (user-defined) type. */
  void visit_custom(fmt::internal::Arg::CustomValue c) { c.format(&formatter_, c.value, &format_); }
};

/** The default argument formatter. */
template <typename Char> class ArgFormatter : public BasicArgFormatter<ArgFormatter<Char>, Char, fmt::FormatSpec> {
public:
  /** Constructs an argument formatter object. */
  ArgFormatter(BasicFormatter<Char> &formatter, fmt::FormatSpec &spec, const Char *fmt)
      : BasicArgFormatter<ArgFormatter<Char>, Char, fmt::FormatSpec>(formatter, spec, fmt) {}
};

template <typename CharType, typename ArgFormatter> class BasicFormatter : private fmt::internal::FormatterBase {
public:
  /** The character type for the output. */
  typedef CharType Char;

private:
  fmt::BasicWriter<Char> &writer_;
  fmt::internal::ArgMap<Char> map_;

  FMT_DISALLOW_COPY_AND_ASSIGN(BasicFormatter);

  using fmt::internal::FormatterBase::get_arg;

  // Checks if manual indexing is used and returns the argument with
  // specified name.
  fmt::internal::Arg get_arg(fmt::BasicStringRef<Char> arg_name, const char *&error);

  // Parses argument index and returns corresponding argument.
  fmt::internal::Arg parse_arg_index(const Char *&s);

  // Parses argument name and returns corresponding argument.
  fmt::internal::Arg parse_arg_name(const Char *&s);

  static std::vector<std::string> formats;

  std::vector<Range> tainted_ranges;
  std::vector<size_t> associated_to_range_indexes;

public:
  /**
   \rst
   Constructs a ``BasicFormatter`` object. References to the arguments and
   the writer are stored in the formatter object so make sure they have
   appropriate lifetimes.
   \endrst
   */
  BasicFormatter(const fmt::ArgList &args, fmt::BasicWriter<Char> &w)
      : fmt::internal::FormatterBase(args), writer_(w) {}

  /** Returns a reference to the writer associated with this formatter. */
  fmt::BasicWriter<Char> &writer() { return writer_; }

  /** Formats stored arguments and writes the output to the writer. */
  void format(fmt::BasicCStringRef<Char> format_str);

  // Formats a single argument and advances format_str, a format string pointer.
  const Char *format(const Char *&format_str, const fmt::internal::Arg &arg, size_t arg_idx);

  const std::vector<Range> &get_tainted_ranges() const { return tainted_ranges; }

  const std::vector<size_t> &get_associated_to_range_indexes() const { return associated_to_range_indexes; }
};

template <typename Char, typename AF> std::vector<std::string> BasicFormatter<Char, AF>::formats = {"safe"};

template <typename Char, typename AF>
inline fmt::internal::Arg BasicFormatter<Char, AF>::get_arg(fmt::BasicStringRef<Char> arg_name, const char *&error) {
  if (check_no_auto_index(error)) {
    map_.init(args());
    const fmt::internal::Arg *arg = map_.find(arg_name);
    if (arg)
      return *arg;
    error = "argument not found";
  }
  return fmt::internal::Arg();
}

template <typename Char, typename AF>
inline fmt::internal::Arg BasicFormatter<Char, AF>::parse_arg_index(const Char *&s) {
  const char *error = FMT_NULL;
  fmt::internal::Arg arg =
      *s < '0' || *s > '9' ? next_arg(error) : get_arg(fmt::internal::parse_nonnegative_int(s), error);
  if (error) {
    FMT_THROW(fmt::FormatError(*s != '}' && *s != ':' ? "invalid format string" : error));
  }
  return arg;
}

template <typename Char, typename AF>
inline fmt::internal::Arg BasicFormatter<Char, AF>::parse_arg_name(const Char *&s) {
  assert(fmt::internal::is_name_start(*s));
  const Char *start = s;
  Char c;
  do {
    c = *++s;
  } while (fmt::internal::is_name_start(c) || ('0' <= c && c <= '9'));
  const char *error = FMT_NULL;
  fmt::internal::Arg arg = get_arg(fmt::BasicStringRef<Char>(start, s - start), error);
  if (error)
    FMT_THROW(fmt::FormatError(error));
  return arg;
}

template <typename Char, typename AF> void BasicFormatter<Char, AF>::format(fmt::BasicCStringRef<Char> format_str) {
  const Char *s = format_str.c_str();
  const Char *start = s;
  size_t next_arg_idx = 0;
  while (*s) {
    Char c = *s++;
    if (c != '{' && c != '}')
      continue;
    if (*s == c) {
      write(writer_, start, s);
      start = ++s;
      continue;
    }
    if (c == '}')
      FMT_THROW(fmt::FormatError("unmatched '}' in format string"));
    write(writer_, start, s - 1);
    const Char *s_idx = s;
    size_t idx = *s_idx < '0' || *s_idx > '9' ? next_arg_idx++ : fmt::internal::parse_nonnegative_int(s_idx);
    fmt::internal::Arg arg = parse_arg_index(s);
    start = s = format(s, arg, idx);
  }
  write(writer_, start, s);
}

template <typename Char, typename ArgFormatter>
const Char *BasicFormatter<Char, ArgFormatter>::format(const Char *&format_str, const fmt::internal::Arg &arg,
                                                       size_t arg_idx) {
  using fmt::internal::Arg;
  const Char *s = format_str;
  typename ArgFormatter::SpecType spec;

  bool safe = false;
  size_t lowerBound = writer_.size();

  if (*s == ':') {
    if (arg.type == Arg::CUSTOM) {
      arg.custom.format(this, arg.custom.value, &s);
      return s;
    }
    ++s;

    std::vector<char> buffer;
    const Char *t = s;
    Char cs = *t;
    while (*t) {
      if (cs == ':' || cs == '}') {
        break;
      }
      buffer.push_back(cs);
      cs = *(++t);
    }
    if (!buffer.empty()) {
      auto str = std::string(buffer.begin(), buffer.end());
      if (std::find(formats.begin(), formats.end(), str) != formats.end()) {
        safe = true;
        s += str.length();
        if (cs == ':') {
          ++s;
        }
      }
    }

    // Parse fill and alignment.
    if (Char c = *s) {
      const Char *p = s + 1;
      spec.align_ = fmt::ALIGN_DEFAULT;
      do {
        switch (*p) {
        case '<':
          spec.align_ = fmt::ALIGN_LEFT;
          break;
        case '>':
          spec.align_ = fmt::ALIGN_RIGHT;
          break;
        case '=':
          spec.align_ = fmt::ALIGN_NUMERIC;
          break;
        case '^':
          spec.align_ = fmt::ALIGN_CENTER;
          break;
        }
        if (spec.align_ != fmt::ALIGN_DEFAULT) {
          if (p != s) {
            if (c == '}')
              break;
            if (c == '{')
              FMT_THROW(fmt::FormatError("invalid fill character '{'"));
            s += 2;
            spec.fill_ = c;
          } else
            ++s;
          if (spec.align_ == fmt::ALIGN_NUMERIC)
            require_numeric_argument(arg, '=');
          break;
        }
      } while (--p >= s);
    }

    // Parse sign.
    switch (*s) {
    case '+':
      check_sign(s, arg);
      spec.flags_ |= fmt::SIGN_FLAG | fmt::PLUS_FLAG;
      break;
    case '-':
      check_sign(s, arg);
      spec.flags_ |= fmt::MINUS_FLAG;
      break;
    case ' ':
      check_sign(s, arg);
      spec.flags_ |= fmt::SIGN_FLAG;
      break;
    }

    if (*s == '#') {
      require_numeric_argument(arg, '#');
      spec.flags_ |= fmt::HASH_FLAG;
      ++s;
    }

    // Parse zero flag.
    if (*s == '0') {
      require_numeric_argument(arg, '0');
      spec.align_ = fmt::ALIGN_NUMERIC;
      spec.fill_ = '0';
      ++s;
    }

    // Parse width.
    if ('0' <= *s && *s <= '9') {
      spec.width_ = fmt::internal::parse_nonnegative_int(s);
    } else if (*s == '{') {
      ++s;
      Arg width_arg = fmt::internal::is_name_start(*s) ? parse_arg_name(s) : parse_arg_index(s);
      if (*s++ != '}')
        FMT_THROW(fmt::FormatError("invalid format string"));
      fmt::ULongLong value = 0;
      switch (width_arg.type) {
      case Arg::INT:
        if (width_arg.int_value < 0)
          FMT_THROW(fmt::FormatError("negative width"));
        value = width_arg.int_value;
        break;
      case Arg::UINT:
        value = width_arg.uint_value;
        break;
      case Arg::LONG_LONG:
        if (width_arg.long_long_value < 0)
          FMT_THROW(fmt::FormatError("negative width"));
        value = width_arg.long_long_value;
        break;
      case Arg::ULONG_LONG:
        value = width_arg.ulong_long_value;
        break;
      default:
        FMT_THROW(fmt::FormatError("width is not integer"));
      }
      unsigned max_int = (std::numeric_limits<int>::max)();
      if (value > max_int)
        FMT_THROW(fmt::FormatError("number is too big"));
      spec.width_ = static_cast<int>(value);
    }

    // Parse precision.
    if (*s == '.') {
      ++s;
      spec.precision_ = 0;
      if ('0' <= *s && *s <= '9') {
        spec.precision_ = fmt::internal::parse_nonnegative_int(s);
      } else if (*s == '{') {
        ++s;
        Arg precision_arg = fmt::internal::is_name_start(*s) ? parse_arg_name(s) : parse_arg_index(s);
        if (*s++ != '}')
          FMT_THROW(fmt::FormatError("invalid format string"));
        fmt::ULongLong value = 0;
        switch (precision_arg.type) {
        case Arg::INT:
          if (precision_arg.int_value < 0)
            FMT_THROW(fmt::FormatError("negative precision"));
          value = precision_arg.int_value;
          break;
        case Arg::UINT:
          value = precision_arg.uint_value;
          break;
        case Arg::LONG_LONG:
          if (precision_arg.long_long_value < 0)
            FMT_THROW(fmt::FormatError("negative precision"));
          value = precision_arg.long_long_value;
          break;
        case Arg::ULONG_LONG:
          value = precision_arg.ulong_long_value;
          break;
        default:
          FMT_THROW(fmt::FormatError("precision is not integer"));
        }
        unsigned max_int = (std::numeric_limits<int>::max)();
        if (value > max_int)
          FMT_THROW(fmt::FormatError("number is too big"));
        spec.precision_ = static_cast<int>(value);
      } else {
        FMT_THROW(fmt::FormatError("missing precision specifier"));
      }
      if (arg.type <= Arg::LAST_INTEGER_TYPE || arg.type == Arg::POINTER) {
        FMT_THROW(fmt::FormatError(fmt::format("precision not allowed in {} format specifier",
                                               arg.type == Arg::POINTER ? "pointer" : "integer")));
      }
    }

    // Parse type.
    if (*s != '}' && *s != ':' && *s)
      spec.type_ = static_cast<char>(*s++);

    if (*s == ':') {
      ++s;
      std::vector<char> buffer;
      t = s;
      cs = *t;
      while (*t) {
        if (cs == '}') {
          break;
        }
        buffer.push_back(cs);
        cs = *(++t);
      }
      if (!buffer.empty()) {
        auto str = std::string(buffer.begin(), buffer.end());
        if (std::find(formats.begin(), formats.end(), str) != formats.end() && !safe) {
          safe = true;
          s += str.length();
        } else {
          FMT_THROW(fmt::FormatError("missing '}' in format string"));
        }
      }
    }
  }

  if (*s++ != '}')
    FMT_THROW(fmt::FormatError("missing '}' in format string"));

  // Format argument.
  ArgFormatter(*this, spec, s - 1).visit(arg);

  if (!safe) {
    tainted_ranges.push_back(Range(lowerBound, writer_.size()));
    associated_to_range_indexes.push_back(arg_idx);
  }

  return s;
}

} // namespace injections
} // namespace protection

#endif // PROTECTION_FORMAT_H
