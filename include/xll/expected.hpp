#ifndef XLL_EXPECTED_HPP
#define XLL_EXPECTED_HPP

#include <type_traits>
#include <cassert>
#include <stdexcept>
#include <eggs/variant.hpp>

namespace xll
{

struct unexpect_t {};
inline unexpect_t unexpect()
{
  return {};
}

template <typename T>
struct unexpected
{
  T value;
};

template <typename T>
unexpected<T> make_unexpected(T&& value)
{
  return unexpected<typename std::remove_reference<T>::type>{
    std::forward<T>(value)};
}

template <typename T, typename E>
class expected
{
public:
  expected(expected&) = default;
  expected(expected&&) = default;

  expected& operator=(expected&) = default;
  expected& operator=(expected&&) = default;


  template <typename T2>
  expected(T2&& v)
    : _value(success_type{std::forward<T2>(v)})
  {
  }

  template <typename T2>
  expected(unexpected<T2> v)
    : _value(error_type{std::move(v.value)})
  {
  }

  template <typename T2>
  expected(unexpect_t(*)(), T2&& v)
    : _value(error_type{std::forward<T2>(v)})
  {
  }

  T operator*() const noexcept
  {
    const success_type* value = _value.template target<success_type>();
    assert(value);
    return value->v;
  }

  E error() const noexcept
  {
    const error_type* value = _value.template target<error_type>();
    assert(value);
    return value->v;
  }

  explicit operator bool() const noexcept
  {
    return _value.which() == 0;
  }

private:
  struct success_type { T v; };
  struct error_type { E v; };

  eggs::variant<success_type, error_type> _value;
};

}

#endif
