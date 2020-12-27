#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Constant.hpp>

namespace Contextual::Details
{
  class Functor
  {
    static constexpr auto askFMap =
      asksC3(
        []<typename Context, typename F, typename T>
        (Context, F&& f, T&& mx){ return Context::fMap(forward<F>(f), forward<T>(mx)); });

    class FMap : public Static_curried<FMap,Nat<2>> {
    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx){
        return
          letC(askFMap, [=](auto fMap){ return
          letC(injest(cmx),[=](auto mx){ return
                returnC(fMap(f, mx)); }); });
      }
    }; // end of class FMap


    class Homogenize : public Static_curried<Homogenize,Nat<2>> {
    public:
      template<typename T, typename U>
      static constexpr auto
      call(T&& x, U&& my){ return fMap(constant(forward<T>(x)), forward<U>(my)); }
    };

  public:

    static constexpr FMap fMap{};
    static constexpr Homogenize homogenize{};

    static constexpr auto letF =
      []<typename T, typename F>
      (T&& mx, F&& f){ return fMap(forward<F>(f), forward<T>(mx)); };

    static constexpr auto liftM =
      curry<2>([]<typename F, typename A>
               (F&& f, A&& mx){ return fApply(pure(forward<f>(f)),forward<A>(mx)); });

  }; // end of class Functor

} // end of namespace Contextual::Details
