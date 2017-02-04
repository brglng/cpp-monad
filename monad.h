#include <iostream>
#include <optional>

namespace monad {

template<template<typename> typename F>
struct Functor {
  template<typename A, typename B>
  static auto fmap(B (*)(A), F<A>) -> F<B>;
};

template<template<typename> typename F, typename A, typename B>
inline auto fmap(B (*func)(A), F<A> something) -> F<B> {
  return Functor<F>::fmap(func, something);
}

template<>
struct Functor<std::optional> {
  template<typename A, typename B>
  static auto fmap(B (*func)(A), std::optional<A> something) -> std::optional<B> {
    if (something.has_value()) {
      return std::optional<B>(func(*something));
    } else {
      return std::nullopt;
    }
  }
};

template<template<typename> typename F>
struct Applicative {
  template<typename A>
  static auto pure(A a) -> F<A>;

  template<typename A, typename B>
  static auto compose(F<B (*)(A)>, F<A>) -> F<B>;
};

template<template<typename> typename F, typename A>
inline auto pure(A a) -> F<A> {
  return Applicative<F>::pure(a);
}

template<template<typename> typename F, typename A, typename B>
inline auto operator%(F<B (*)(A)> func, F<A> something) -> F<B> {
  return Applicative<F>::compose(func, something);
}

template<>
struct Applicative<std::optional> {
  template<typename A>
  static auto pure(A a) -> std::optional<A> {
    return std::optional<A>(a);
  }

  template<typename A, typename B>
  static auto compose(std::optional<B (*)(A)> func, std::optional<A> something) -> std::optional<B> {
    if (!func.has_value()) {
      return std::nullopt;
    } else {
      return fmap(*func, something);
    }
  }
};

template<template<typename> typename M>
struct Monad {
  template<typename A>
  static auto return_(A) -> M<A>;

  template<typename A, typename B>
  static auto bind(M<A>, M<B> (*)(A)) -> M<B>;
};

template<>
struct Monad<std::optional> {
  template<typename A>
  static auto return_(A x) -> std::optional<A> {
    return std::optional<A>(x);
  }

  template<typename A, typename B>
  static auto bind(std::optional<A> x, std::optional<B> (*func)(A)) -> std::optional<B> {
    if (!x.has_value()) {
      return std::nullopt;
    } else {
      return (*func)(*x);
    }
  }
};

// >>= is left associative, so use |
template<template<typename> typename M, typename A, typename B>
inline auto operator|(M<A> x, M<B> (*func)(A)) -> M<B> {
  return Monad<M>::bind(x, func);
}

template<typename T>
using Maybe = std::optional<T>;

auto Nothing = std::nullopt;

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::optional<T>& o) {
  if (o) {
    return stream << "Maybe " << *o;
  } else {
    return stream << "Nothing";
  }
}

} // namespace monad
