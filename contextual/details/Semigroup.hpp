#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Magma.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details {

  template<typename Context>
  concept HasOp_ = requires
  {
    {Context::op_};
  };

  template<typename Context>
  concept MissingOp_ = !HasOp_<Context>;

  template<typename Context>
  concept HasMinimalSemigroup = HasMinimalMagma<Context> || HasOp_<Context>;

  template<typename Context>
  concept MissingMinimalSemigroup = !HasMinimalSemigroup<Context>;

  template<typename Context>
  concept HasSemigroup = HasMagma<Context> && HasOp_<Context>;

  template<typename Context>
  concept MissingSemigroup = !HasSemigroup<Context>;

  class MixinSemigroup : public Static_curried<MixinSemigroup, Nat<1>>
  {

    //   ___
    //  / _ \ _ __
    // | (_) | '_ \.
    //  \___/| .__/
    //       |_|
    class MixinOp : public Static_curried<MixinOp, Nat<1>>
    {
      template<HasOp_ Base>
      class Result : public Base
      {
        class Op : public Static_curried<Op, Nat<2>>
        {
        public:
          template<typename T, typename U>
          static constexpr auto
          call(T&& x, U&& y)
          {
            return Base::op_(std::forward<T>(x), std::forward<U>(y));
          }
        };

      public:
        using Base::Base;
        static constexpr Op op{};
      }; // end of class Result
    public:
      template<HasOp_ Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasOp<Result<Base>>);
      }
    } static constexpr mixinOp{};

    //   ___
    //  / _ \ _ __
    // | (_) | '_ \.
    //  \___/| .__/__
    //       |_| |___|
    class MixinOp_ : public Static_curried<MixinOp_, Nat<1>>
    {
      template<HasOp Base>
      class Result : public Base
      {
        class Op_ : public Static_callable<Op_>
        {
        public:
          template<typename T, typename U, typename... Vs>
          static constexpr auto
          call(T&& x, U&& y, Vs&&... zs)
          {
            if constexpr (count_types<Vs...>() == 0) {
              return Base::op(x, y);
            } else {
              return call(Base::op(x, y), zs...);
            }
          }
        };

      public:
        using Base::Base;
        static constexpr Op_ op_{};
      };

    public:
      template<HasOp Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasOp_<Result<Base>>);
      }
    } static constexpr mixinOp_{};

    ////////////////////////////////////////////////////////////////////////

  public:
    template<HasMinimalSemigroup Base>
    static constexpr auto
    call(Type<Base>)
    {
      if constexpr (MissingOp<Base>) {
        return call(mixinOp(type<Base>));

      } else if constexpr (MissingOp_<Base>) {
        return call(mixinOp_(type<Base>));

      } else if constexpr (MissingMagma<Base>) {
        return call(mixinMagma(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasSemigroup<Base>);
      }
    }
  } constexpr mixinSemigroup{};

  class ProtoSemigroup : public Magma
  {};

  class Semigroup : public Derive<ProtoSemigroup, MixinSemigroup>
  {};

} // end of namespace Contextual::Details
