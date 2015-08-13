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
class expected;

template <typename T>
struct expected_traits;

template <typename T, typename E>
struct expected_traits<expected<T, E>>
{
  using success_type = T;
  using error_type = E;
};

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
    : _value(success_type{T(std::forward<T2>(v))})
  {
  }

  template <typename T2>
  expected(unexpected<T2> v)
    : _value(error_type{E(std::move(v.value))})
  {
  }

  template <typename T2>
  expected(unexpect_t(*)(), T2&& v)
    : _value(error_type{E(std::forward<T2>(v))})
  {
  }

  const T& operator*() const noexcept
  {
    const success_type* value = _value.template target<success_type>();
    assert(value);
    return value->v;
  }

  T& operator*() noexcept
  {
    success_type* value = _value.template target<success_type>();
    assert(value);
    return value->v;
  }

  E error() const noexcept
  {
    const error_type* value = _value.template target<error_type>();
    assert(value);
    return value->v;
  }

  template <typename F>
  auto map(F&& func) -> expected<decltype(func(std::declval<T>())), E>
  {
    if (*this)
      return {func(**this)};
    else
      return {unexpect, error()};
  }

  template <typename F,
      typename V = typename std::enable_if<
          std::is_same<typename expected_traits<decltype(
                           std::declval<F>()(std::declval<T>()))>::error_type,
              E>::value>::type>
  auto bind(F&& func)
      -> expected<typename expected_traits<decltype(
                      std::declval<F>()(std::declval<T>()))>::success_type,
          E>
  {
    if (*this)
      return func(**this);
    else
      return {unexpect, error()};
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
