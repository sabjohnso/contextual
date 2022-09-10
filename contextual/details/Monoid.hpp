#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Semigroup.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details {

  template<typename Context>
  concept HasMEmpty = requires
  {
    {Context::mEmpty};
  };

  template<typename Context>
  concept MissingMEmpty = !HasMEmpty<Context>;

  template<typename Context>
  concept HasMAppend = requires
  {
    {Context::mAppend};
  };

  template<typename Context>
  concept MissingMAppend = !HasMAppend<Context>;

  template<typename Context>
  concept HasMAppend_ = requires
  {
    {Context::mAppend_};
  };

  template<typename Context>
  concept MissingMAppend_ = !HasMAppend_<Context>;

  template<typename Context>
  concept HasMinimalMonoid = HasMEmpty<Context> &&
    (HasOp<Context> || HasOp_<Context> || HasMAppend<Context> ||
     HasMAppend_<Context>);

  template<typename Context>
  concept MissingMinimalMonoid = !HasMinimalMonoid<Context>;

  template<typename Context>
  concept HasMonoid =
    HasSemigroup<Context> && HasMAppend<Context> && HasMAppend_<Context>;

  template<typename Context>
  concept MissingMonoid = !HasMonoid<Context>;

  class MixinMonoid : public Static_curried<MixinMonoid, Nat<1>>
  {
    //  __  __   _                          _
    // |  \/  | /_\  _ __ _ __  ___ _ _  __| |
    // | |\/| |/ _ \| '_ \ '_ \/ -_) ' \/ _` |
    // |_|  |_/_/ \_\ .__/ .__/\___|_||_\__,_|
    //              |_|  |_|
    class MixinMAppendFromMAppend_
      : public Static_curried<MixinMAppendFromMAppend_, Nat<1>>
    {

      template<HasMAppend_ Base>
      class Result : public Base
      {
        class MAppend : public Static_curried<MAppend, Nat<2>>
        {
        public:
          template<typename T, typename U>
          static constexpr auto
          call(T&& x, U&& y)
          {
            return Base::mappend_(forward<T>(x), forward<U>(y));
          }
        };

      public:
        using Base::Base;
        static constexpr MAppend mAppend{};
      };

    public:
      template<HasMAppend_ Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasMAppend<Result<Base>>);
      }
    } static constexpr mixinMAppendFromMAppend_{};

    class MixinMAppendFromOp : public Static_curried<MixinMAppendFromOp, Nat<1>>
    {
      template<HasOp Base>
      class Result : public Base
      {
      public:
        using Base::Base;
        static constexpr auto mAppend = Base::op;
      };

    public:
      template<HasOp Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasMAppend<Result<Base>>);
      }
    } static constexpr mixinMAppendFromOp{};

    class MixinMAppendFromOp_
      : public Static_curried<MixinMAppendFromOp_, Nat<1>>
    {
      template<HasOp_ Base>
      class Result : public Base
      {
        class MAppend : public Static_curried<MAppend, Nat<2>>
        {
        public:
          template<typename T, typename U>
          static constexpr auto
          call(T&& x, U&& y)
          {
            return Base::op_(forward<T>(x), forward<T>(y));
          }
        };

      public:
        using Base::Base;
        static constexpr MAppend mAppend{};
      };

    public:
      template<HasOp_ Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasMAppend<Result<Base>>);
      }
    } static constexpr mixinMAppendFromOp_{};

    //  __  __   _                          _
    // |  \/  | /_\  _ __ _ __  ___ _ _  __| |
    // | |\/| |/ _ \| '_ \ '_ \/ -_) ' \/ _` |
    // |_|  |_/_/ \_\ .__/ .__/\___|_||_\__,_|__
    //              |_|  |_|                |___|
    class MixinMAppend_ : public Static_curried<MixinMAppend_, Nat<1>>
    {
      template<HasMAppend Base>
      class Result : public Base
      {
        class MAppend_ : public Static_callable<MAppend_>
        {
        public:
          static constexpr auto
          call()
          {
            return Base::mEmpty;
          }

          template<typename T, typename U>
          static constexpr auto
          call(T&& x, U&& y)
          {
            return Base::mAppend(forward<T>(x), forward<U>(y));
          }

          template<typename T, typename U, typename V, typename... Vs>
          static constexpr auto
          call(T&& x, U&& y, V&& z, Vs&&... zs)
          {
            return call(
              Base::mAppend(forward<T>(x), forward<T>(y)),
              forward<V>(z),
              forward<Vs>(zs)...);
          }
        };

      public:
        using Base::Base;
        static constexpr MAppend_ mAppend_{};
      };

    public:
      template<HasMAppend Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasMAppend_<Result<Base>>);
      }
    } static constexpr mixinMAppend_{};

    //   ___
    //  / _ \ _ __
    // | (_) | '_ \.
    //  \___/| .__/
    //       |_|
    class MixinOp : public Static_curried<MixinOp, Nat<1>>
    {
      template<HasMAppend Base>
      class Result : public Base
      {
      public:
        using Base::Base;
        static constexpr auto op = Base::mAppend;
      };

    public:
      template<HasMAppend Base>
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
      template<HasMAppend Base>
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
              return Base::mAppend(forward<T>(x), forward<U>(y));
            } else {
              return call(Base::mAppend(x, y), forward<Vs>(zs)...);
            }
          }
        };

      public:
        using Base::Base;
        static constexpr Op_ op_{};
      };

    public:
      template<HasMAppend Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasOp_<Result<Base>>);
      }
    } static constexpr mixinOp_{};

    ////////////////////////////////////////////////////////////////////////
  public:
    template<HasMinimalMonoid Base>
    static constexpr auto
    call(Type<Base>)
    {
      if constexpr (MissingMAppend<Base>) {
        if constexpr (HasMAppend_<Base>) {
          return call(mixinMAppendFromMAppend_(type<Base>));

        } else if constexpr (HasOp<Base>) {
          return call(mixinMAppendFromOp(type<Base>));

        } else if constexpr (HasOp_<Base>) {
          return call(mixinMAppendFromOp_(type<Base>));

        } else {
          return call(mixinMAppendFromOp(type<Base>));
        }
      } else if constexpr (MissingMAppend_<Base>) {
        return call(mixinMAppend_(type<Base>));

      } else if constexpr (MissingOp<Base>) {
        return call(mixinOp(type<Base>));

      } else if constexpr (MissingOp_<Base>) {
        return call(mixinOp_(type<Base>));

      } else if constexpr (MissingSemigroup<Base>) {
        return call(mixinSemigroup(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonoid<Base>);
      }
    }
  } constexpr mixinMonoid{};

  class ProtoMonoid : public Semigroup
  {
  public:
    static constexpr auto mEmpty =
      asksC([]<typename Context>(Context) { return Context::mEmpty; });
  };

  class Monoid : public Derive<ProtoMonoid, MixinMonoid>
  {};

} // end of namespace Contextual::Details
