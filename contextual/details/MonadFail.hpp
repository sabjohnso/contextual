#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasFail = requires { {Context::fail};};

  template<typename Context>
  concept MissingFail = ! HasFail<Context>;

  template<typename Context>
  concept HasMinimalMonadFail = HasMinimalMonad<Context> && HasFail<Context>;

  template<typename Context>
  concept HasMonadFailCore = HasMonadCore<Context> && HasFail<Context>;

  template<typename Context>
  concept MissingMonadFailCore = ! HasMonadFailCore<Context>;

  template<typename Context>
  concept HasMonadFailUtility = HasMonadUtility<Context> && HasFail<Context>;

  template<typename Context>
  concept MissingMonadFailUtility = ! HasMonadFailUtility<Context>;



  class MixinMonadFail : public Static_curried<MixinMonadFail, Nat<1>>{
  public:
    template<HasMinimalMonadFail Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadCore<Base>){
        return call(mixinMonad(type<Base>));
      } else {
        return type<Base>;
      }
    }
  } constexpr mixinMonadFail{};


  class MixinMonadFailUtility : public Static_curried<MixinMonadFailUtility,Nat<1>> {
  public:
    template<HasMonadFailCore Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadUtility<Base>){
        return call(mixinMonadUtility(type<Base>));
      } else {
        return type<Base>;
        static_assert(HasMonadFailUtility<Base>);
      }
    }
  } constexpr mixinMonadFailUtility{};

  class MonadFail : public Monad
  {

    class Fail : public Static_curried<Fail, Nat<1>> {
      static constexpr auto askFail =
        asksC2([]<typename Context, typename T>
              (Context, Type<T>){return Context::fail(type<T>);});
    public:
      template<typename T>
      static constexpr auto
      call(Type<T>){
        return
          letC(askFail, [](auto fail){ return
                returnC(fail(type<T>)); });
      }
    }; // end of class Fail

  public:

    static constexpr Fail fail{};

  }; // end of class MonadFail

  static_assert(HasMonadFailUtility<MonadFail>);




} // end of namespace Contextual::Details
