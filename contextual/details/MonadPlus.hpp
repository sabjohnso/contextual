#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/MonadFail.hpp>

namespace Contextual::Details
{

  class MonadPlus : public MonadFail
  {
    constexpr static auto askMPlus =
      asksC3(
        []<typename Context, typename T, typename U>
        (Context, T&& mx, U&& my){
          return Context::mPlus(forward<T>(mx), forward<T>(my));
        });

    class MPlus : public Static_curried<MPlus, Nat<2>> {
    public:
      template<typename T, typename U>
      static constexpr auto
      call(T&& cmx, U&& cmy){
        return
          letC(askMPlus, [=](auto mplus){ return
          letC(injest(cmx), [=](auto mx){ return
          letC(injest(cmy), [=](auto my){ return
                returnC(mplus(mx, my)); }); }); });
      }
    };

  public:

    template<typename T>
    static constexpr auto mZero = fail<T>;

    static constexpr MPlus mPlus{};

  }; // end of class MonadPlus

} // end of namespace Contextual::Details
