#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>

namespace Contextual::Details
{
  class Magma
  {
    static constexpr auto askOp =
      asksC3([]<typename Context, typename T, typename U>
             (Context, T&& x, U&& y){ return Context::op(forward<T>(x), forward<U>(y)); });

    class Op : public Static_curried<Op, Nat<2>> {
    public:
      template<typename T, typename U>
      static constexpr auto
      call(T&& mx, U&& my){
        return
          letC(askOp,      [=](auto op){ return
          letC(injest(mx), [=](auto x){ return
          letC(injest(my), [=](auto y){ return
                returnC(op(x, y)); }); }); });
      }
    }; // end of class Op
  public:
    static constexpr auto op = Op{};
  }; // end of class Magma
} // end of namespace Contextual::Details
