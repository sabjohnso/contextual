#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Derive.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasOp = requires{{Context::op};};

  template<typename Context>
  concept MissingOp = ! HasOp<Context>;

  template<typename Context>
  concept HasMinimalMagma = HasOp<Context>;

  template<typename Context>
  concept MissingMinimalMagma = ! HasMinimalMagma<Context>;

  template<typename Context>
  concept HasMagma = HasMinimalMagma<Context>;

  template<typename Context>
  concept MissingMagma = ! HasMagma<Context>;

  class MixinMagma {
  public:
    template<HasMinimalMagma Base>
    static constexpr auto
    call(Type<Base>){
      return type<Base>;
      static_assert(HasMagma<Base>);
    }
  } constexpr mixinMagma{};

  class ProtoMagma {
    class Op : public Static_curried<Op, Nat<2>>{
      static constexpr auto askOp =
        asksC3([]<typename Context, typename T, typename U>
               (Context, T&& x, U&& y){
                 return Context::op(forward<T>(x), forward<U>(y));
               });
    public:
      template<typename T, typename U>
      static constexpr auto
      call(T&& cx, U&& cy){ return
          letC(askOp,      [=](auto op){ return
          letC(injest(cx), [=](auto  x){ return
          letC(injest(cy), [=](auto  y){ return
                returnC(op(x, y)); }); }); });
      }
    };
  public:
    static constexpr Op op{};
  };

  class Magma : public ProtoMagma
  {};


} // end of namespace Contextual::Details
