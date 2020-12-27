#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{

  class MonadState : public Monad
  {
    static constexpr auto askPut =
      asksC2([]<typename Context, typename S>
             (Context, S&& s){ return Context::put(forward<S>(s)); });

    class Put : public Static_curried<Put,Nat<1>> {
    public:
      template<typename S>
      static constexpr auto
      call( S&& s){
        return
          letC(askPut, [s = forward<S>(s)](auto put){ return
                returnC(put(s)); });
      }
    };

  public:
    static constexpr auto get = asksC([]<typename Context> (Context){ return Context::get; });
    static constexpr Put put{};
    static constexpr auto select =
      curry<1>([]<typename F>(F&& f){ return fMap(forward<F>(f), get); });

    static constexpr auto modify =
      curry<1>([]<typename F>
               (F&& f){ return flatMap(compose(put, f), get); });




  }; // end of class MonadState

} // end of namespace Contextual::Details
