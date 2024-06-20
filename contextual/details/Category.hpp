#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Contextual.hpp>

namespace Contextual::Details
{

  class Category{

    static constexpr auto askCat =
      asksC2(
        []<typename Context, typename F>
        (Context, F&& f){ return Context::cat(std::forward<F>(f)); });

    static constexpr auto askCompose =
      asksC3(
        []<typename Context, typename F, typename G>
          (Context, F&& f, G&& g){ return Context::compose(std::forward<F>(f), std::forward<G>(g)); });

    class Cat : public Static_curried<Cat, Nat<1>>{
    public:
      template<typename F>
      static constexpr auto
      call(F&& f){
        return
          letC(askCat, [=](auto cat){ return
                returnC(cat(f)); });
      }
    }; // end of class Cat


    class Compose : public Static_curried<Compose, Nat<2>>{
    public:
      template<typename F, typename G>
      static constexpr auto
      call(F&& cf, G&& cg){
        return
          letC(askCompose,[=](auto compose){ return
          letC(injest(cf),[=](auto f){ return
          letC(injest(cg),[=](auto g ){ return
                returnC(compose(f, g)); }); }); });
      }
    }; // end of class Compose



  public:

    static constexpr Cat cat{};
    static constexpr Compose compose{};
    static constexpr auto identity =
      asksC([]<typename Context>
            (Context){ return Context::identity; });

  }; // end of class Category



} // end of namespace Contextual::Details
