#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{

  class MonadReader : public Monad
  {
  private:

    static constexpr auto askLocal =
      asksC3(
        []<typename Context, typename F, typename T>
        (Context, F&& f, T&& mx){
          return Context::local(forward<F>(f), forward<T>(mx));
        });

    class Local : public Static_curried<Local,Nat<2>>{
    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx){
        return
          letC(askLocal,    [=](auto local){ return
          letC(injest(cmx), [=](auto  mx  ){ return
                returnC(local(f, mx)); }); });
      }
    }; // end of class Local
  public:

    static constexpr auto ask =
      asksC([]<typename Context>(Context){ return Context::ask(); });

    static constexpr auto asks =
      []<typename F>(F&& f){ return fMap(forward<F>(f), ask); };

    static constexpr Local local{};

  }; // end of class MonadReader

} // end of namespace Contextual::Details
