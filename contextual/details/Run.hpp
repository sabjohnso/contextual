#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Either.hpp>

namespace Contextual::Details
{
  constexpr
  class Run : public Static_curried<Run,Nat<2>>
  {
  public:
    template<typename T, typename F>
    static constexpr auto
    call(T&& x, F&& f){ return std::forward<F>(f)(std::forward<T>(x)); }

    template<
      typename T,
      typename F,
      typename G,
      typename FirstResult = invoke_result_t<F,T>,
      typename SecondResult = invoke_result_t<G,T>,
      typename Result = conditional_t<
        is_same_v<FirstResult,SecondResult>,
        FirstResult, Either<FirstResult,SecondResult> >
      >
    static constexpr auto
    call(T&& x, Either<F,G> const& f) {
      return f.holdsFirst()
        ? Result(f.first()( forward<T>(x)))
        : Result(f.second()( forward<T>(x)));
    }
  }run{};

} // end of namespace Contextual::Details
