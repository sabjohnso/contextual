#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Contextual.hpp>
#include <contextual/details/MonadFail.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details {

  template<typename Context>
  concept HasMZero = requires
  {
    {Context::mZero};
  };

  template<typename Context>
  concept MissingMZero = !HasMZero<Context>;

  template<typename Context>
  concept HasMPlus = requires
  {
    {Context::mPlus};
  };

  template<typename Context>
  concept MissingMPlus = !HasMPlus<Context>;

  template<typename Context>
  concept HasMinimalMonadPlus = HasMinimalMonad<Context> &&
    (HasFail<Context> || HasMZero<Context>)&&HasMPlus<Context>;

  template<typename Context>
  concept HasMonadPlusCore = HasMonadCore<Context> &&
    HasMonadFailCore<Context> && HasMZero<Context> && HasMPlus<Context>;

  template<typename Context>
  concept MissingMonadPlusCore = !HasMonadPlusCore<Context>;

  template<typename Context>
  concept HasMonadPlusUtility =
    HasMonadUtility<Context> && HasMonadPlusCore<Context>;

  template<typename Context>
  concept MissingMonadPlusUtility = !HasMonadPlusUtility<Context>;

  template<typename Context>
  concept HasMonadPlus =
    HasMonadPlusCore<Context> && HasMonadPlusUtility<Context>;

  template<typename Context>
  concept MissingMonadPlus = !HasMonadPlus<Context>;

  class MixinMonadPlusCore : public Static_curried<MixinMonadPlusCore, Nat<1>>
  {

    //  __  __ ____
    // |  \/  |_  /___ _ _ ___
    // | |\/| |/ // -_) '_/ _ \.
    // |_|  |_/___\___|_| \___/
    class MixinMZero : public Static_callable<MixinMZero>
    {
      template<HasFail Base>
      class Result : public Base
      {
      public:
        static constexpr auto mZero = Base::fail;
        using Base::Base;
      };

    public:
      template<HasFail Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasMZero<Result<Base>>);
      }
    } static constexpr mixinMZero{};

    //  ___     _ _
    // | __|_ _(_) |
    // | _/ _` | | |
    // |_|\__,_|_|_|
    class MixinFail : public Static_curried<MixinFail, Nat<1>>
    {
      template<HasMZero Base>
      class Result : public Base
      {
      public:
        static constexpr auto fail = Base::mZero;
        using Base::Base;
      };

    public:
      template<HasMZero Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFail<Result<Base>>);
      }
    } static constexpr mixinFail{};

  public:
    template<HasMinimalMonadPlus Base>
    static constexpr auto
    call(Type<Base>)
    {
      if constexpr (MissingMZero<Base>) {
        return call(mixinMZero(type<Base>));

      } else if constexpr (MissingFail<Base>) {
        return call(mixinFail(type<Base>));

      } else if constexpr (MissingMonadFailCore<Base>) {
        return call(mixinMonadFail(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadPlusCore<Base>);
      }
    }

  } constexpr mixinMonadPlusCore{};

  class MixinMonadPlusUtility
    : public Static_curried<MixinMonadPlusUtility, Nat<1>>
  {
  public:
    template<HasMonadPlusCore Base>
    static constexpr auto
    call(Type<Base>)
    {
      if constexpr (MissingMonadFailUtility<Base>) {
        return call(mixinMonadFailUtility(type<Base>));
      } else {
        return type<Base>;
        static_assert(HasMonadPlusUtility<Base>);
      }
    }
  } constexpr mixinMonadPlusUtility{};

  class MixinMonadPlus : public Static_curried<MixinMonadPlus, Nat<1>>
  {
  public:
    template<HasMinimalMonadPlus Base>
    static constexpr auto
    call(Type<Base>)
    {

      if constexpr (MissingMonadPlusCore<Base>) {
        return call(mixinMonadPlusCore(type<Base>));

      } else if constexpr (MissingMonadPlusUtility<Base>) {
        return call(mixinMonadPlusUtility(type<Base>));

      } else {
        return type<Base>;
      }
    }
  } constexpr mixinMonadPlus{};

  class ProtoMonadPlus : public MonadFail
  {
    constexpr static auto askMPlus = asksC3(
      []<typename Context, typename T, typename U>(Context, T&& mx, U&& my) {
        return Context::mPlus(forward<T>(mx), forward<U>(my));
      });

    class MPlus : public Static_curried<MPlus, Nat<2>>
    {
    public:
      template<typename T, typename U>
      static constexpr auto
      call(T&& cmx, U&& cmy)
      {
        return letC(askMPlus, [=](auto mplus) {
          return letC(injest(cmx), [=](auto mx) {
            return letC(
              injest(cmy), [=](auto my) { return returnC(mplus(mx, my)); });
          });
        });
      }
    };

  public:
    static constexpr MPlus mPlus{};
  }; // end of class MonadPlus

  class MonadPlus
    : public Derive<ProtoMonadPlus, MixinMonadPlus, MixinMonadPlusUtility>
  {};

} // end of namespace Contextual::Details
