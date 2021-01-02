#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Monad.hpp>
#include <contextual/details/Monoid.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details
{

  class MonadWriter : public Monad
  {
    static constexpr auto askTell =
      asksC2([]<typename Context, typename W>(Context, W&& w) {
        return Context::tell(forward<W>(w));
      });

    static constexpr auto askWriter =
      asksC2([]<typename Context, typename Pair>(Context, Pair&& aw) {
          return Context::writer(forward<Pair>(aw));
        });

    static constexpr auto askListen =
      asksC2([]<typename Context, typename T>(Context, T&& aw) {
        return Context::listen(forward<T>(aw));
      });

    static constexpr auto askPass =
      asksC2([]<typename Context, typename Pair>(Context, Pair&& maf) {
        return Context::pass(forward<Pair>(maf));
      });

    class Tell : public Static_curried<Tell, Nat<1>>{
    public:
      template<typename W>
      static constexpr auto
      call(W&& w)
      {
        // clang-format off
        return
        letC(askTell, [=](auto tell) { return
              returnC(tell(w)); });
        // clang-format on
      }
    };

    class Writer : public Static_curried<Writer, Nat<1>>
    {
    public:
      template<typename AW>
      static constexpr auto
      call(AW const& aw)
      {
        // clang-format off
        return
          letC(askWriter, [=](auto writer){ return
                returnC( writer(aw)); });
        // clang-format on
      }
    };

    class Listen : public Static_curried<Listen, Nat<1>>
    {
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmx){
        // clang-format off
        return
          letC(askListen, [=](auto listen){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(listen(mx)); }); });
        // clang-format on
      }
    }; // end of class Listen

    class Pass : public Static_curried<Pass, Nat<1>>
    {
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmx){
        return
          letC(askPass, [=](auto pass){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(pass(mx)); }); });
      }

    }; // end of class Pass

    public :

    static constexpr Tell tell{};
    static constexpr Writer writer{};
    static constexpr Listen listen{};
    static constexpr Pass pass{};

    static constexpr auto censor =
      curry<2>([]<typename F, typename T>(F&& f, T&& mx){
          return pass(fMap([=](auto x){ return pair(x, f); }, mx));
        });

  }; // end of class MonadWriter

} // end of namespace Contextual::Details
