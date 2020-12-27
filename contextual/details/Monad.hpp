#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Identity.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Digest.hpp>
#include <contextual/details/Expel.hpp>
#include <contextual/details/Functor.hpp>
#include <contextual/details/Applicative.hpp>

namespace Contextual::Details
{
  class Monad : public Applicative
  {
    static constexpr auto askFlatMap =
      asksC3([]<typename Context, typename F, typename T>
             (Context, F&& f, T&& mx){
               return Context::flatMap(forward<F>(f), forward<T>(mx)); });

    static constexpr auto askFlatten =
      asksC2([]<typename Context, typename T>
             (Context, T&& mmx){
               return Context::flatten(forward<T>(mmx)); });

    class FlatMap : public Static_curried<FlatMap, Nat<2>>{
    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx){
        return
          letC(askExpel, [=](auto expel){ return
          letC(askFlatMap, [=](auto flatMap){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(flatMap([=](auto x){ return expel(f(x)); }, mx)); }); }); });
      }
    }; // end of class FlatMap

    class Flatten : public Static_curried<Flatten, Nat<1>>{
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmcmx){
        return
          letC(askExpel, [=](auto expel){ return
          letC(askFlatten, [=](auto flatten){ return
          letC(fMap(expel, digest(cmcmx)), [=](auto mmx){ return
                returnC(flatten(mmx)); }); }); });
      }
    }; // end of class Flatten


    class BeginM : public Static_callable<BeginM>{
    public:
      template<typename T, typename ... Ts>
      static constexpr auto
      call( T&& mx, Ts&& ... mxs){
        if constexpr (count_types<Ts...>() == 0){ return mx; }
        else { return flatMap([=](auto){ return call(mxs ...); }, mx); }
      }
    };

  public:

    template<typename Context>
    class MonadicFMap{
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx){
        return Context::flatMap([&](auto x){ return Context::pure(f(x)); }, mx );
      }
    };

    template<typename Context>
    class MonadicFApply{
    public:
      template<typename F, typename T>
      static constexpr auto
      fApply(F&& mf, T&& mx){
        return Context::flatMap(
          [&](auto f){ return Context::fMap(f, mx); },
          mf);
      }
    };

    template<typename Context>
    class MonadicFlatMap{
    public:
      template<typename F, typename T>
      static constexpr auto
      flatMap(F&& f, T&& mx){
        return Context::flatten(Context::fMap(f, mx));
      }
    };

    template<typename Context>
    class MonadicFlatten{
    public:
      template<typename T>
      static constexpr auto
      flatten(T&& mmx){ return Context::flatMap(identity, mmx); }
    };


    static constexpr FlatMap flatMap{};
    static constexpr Flatten flatten{};

    static constexpr auto letM =
      []<typename T, typename F>
      (T&& mx, F&& f){ return flatMap(forward<F>(f), forward<T>(mx)); };

    static constexpr auto returnM = pure;

    static constexpr BeginM beginM{};



  }; // end of class Monad

} // end of namespace Contextual::Details
