#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Identity.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Digest.hpp>
#include <contextual/details/Expel.hpp>
#include <contextual/details/Functor.hpp>
#include <contextual/details/Applicative.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasFlatMap = requires{{Context::flatMap};};

  template<typename Context>
  concept MissingFlatMap = ! HasFlatMap<Context>;

  template<typename Context>
  concept HasFlatten = requires{{Context::flatten};};

  template<typename Context>
  concept MissingFlatten = ! HasFlatten<Context>;

  template<typename Context>
  concept HasMinimalMonad =
    (HasFlatMap<Context> && HasPure<Context>) ||
    (HasFMap<Context> && HasPure<Context> && HasFlatten<Context>);

  template<typename Context>
  concept MissingMinimalMonad = ! HasMinimalMonad<Context>;

  template<typename Context>
  concept HasMonadCore =
    HasFunctorCore<Context> &&
    HasApplicativeCore<Context> &&
    HasFlatMap<Context> &&
    HasFlatten<Context>;

  template<typename Context>
  concept MissingMonadCore = ! HasMonadCore<Context>;

  template<typename Context>
  concept HasReturnM = requires{{Context::returnM};};

  template<typename Context>
  concept MissingReturnM = ! HasReturnM<Context>;

  template<typename Context>
  concept HasLetM = requires{{Context::letM};};

  template<typename Context>
  concept MissingLetM = ! HasLetM<Context>;

  template<typename Context>
  concept HasBeginM = requires{{Context::beginM};};

  template<typename Context>
  concept MissingBeginM = ! HasBeginM<Context>;

  template<typename Context>
  concept HasMonadUtility =
    HasApplicativeUtility<Context> &&
    HasMonadCore<Context> &&
    HasReturnM<Context> &&
    HasLetM<Context> &&
    HasBeginM<Context>;

  template<typename Context>
  concept MissingMonadUtility = ! HasMonadUtility<Context>;


  template<typename Context>
  concept HasFMapAndFlatten = HasFMap<Context> && HasFlatten<Context>;

  template<typename Context>
  concept HasFMapAndFlatMap = HasFMap<Context> && HasFlatMap<Context>;


  template<typename Context>
  concept HasPureAndFlatMap = HasPure<Context> && HasFlatMap<Context>;


  template<typename Context>
  concept HasMonad = HasMonadUtility<Context>;


  /**
   * @brief Mixin monad core methods
   */
  class MixinMonadCore : public Static_curried<MixinMonadCore, Nat<1>>
  {

    //  ___ __  __
    // | __|  \/  |__ _ _ __
    // | _|| |\/| / _` | '_ \
    // |_| |_|  |_\__,_| .__/
    //                 |_|
    class MixinFMap : public Static_curried<MixinFMap, Nat<1>>
    {
      template<HasPureAndFlatMap Base>
      class Result : public Base
      {
        class FMap : public Static_curried<FMap, Nat<2>>
        {
        public:
          template<typename F, typename T>
          static constexpr auto
          call(F&& f, T&& mx)
          {
            return Base::flatMap(compose(Base::pure, forward<F>(f)), forward<T>(mx));
          }
        };
      public:
        using Base::Base;
        static constexpr FMap fMap{};
      };

      public:
      template<HasPureAndFlatMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFMap<Result<Base>>);
      }
    } static constexpr mixinFMap{};

    //  ___ _      _   __  __
    // | __| |__ _| |_|  \/  |__ _ _ __
    // | _|| / _` |  _| |\/| / _` | '_ \
    // |_| |_\__,_|\__|_|  |_\__,_| .__/
    //                            |_|
    /**
     * @brief Mixin the `flatMap` method
     */
    class MixinFlatMap : public Static_curried<MixinFlatMap, Nat<1>>
    {
      template<HasFMapAndFlatten Base>
      class Result : public Base
      {
        class FlatMap : public Static_curried<FlatMap, Nat<2>>
        {
        public:

          template<typename F, typename T>
          static constexpr auto
          call(F&& f, T&& mx)
          {
            return Base::flatten(Base::fMap(forward<F>(f), forward<T>(mx)));
          }
        };

      public:
        using Base::Base;
        static constexpr FlatMap flatMap{};
      };

    public:
      template<HasFMapAndFlatten Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFlatMap<Result<Base>>);
      }
    } static constexpr mixinFlatMap{};


    //  ___ _      _   _
    // | __| |__ _| |_| |_ ___ _ _
    // | _|| / _` |  _|  _/ -_) ' \
    // |_| |_\__,_|\__|\__\___|_||_|
    /**
     * @brief Mixin the `flatten`  method
     */
    class MixinFlatten : public Static_curried<MixinFlatten, Nat<1>>
    {
      template<HasFlatMap Base>
      class Result : public Base
      {
        class Flatten : public Static_curried<Flatten, Nat<1>>
        {
        public:

          template<typename T>
          static constexpr auto
          call(T&& mmx)
          {
            return Base::flatMap(identity, forward<T>(mmx));
          }
        };

      public:
        using Base::Base;
        static constexpr Flatten flatten{};
      };

      public:
      template<HasFMapAndFlatMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFlatten<Result<Base>>);
      }
    } static constexpr mixinFlatten{};

    //  ___ _             _
    // | __/_\  _ __ _ __| |_  _
    // | _/ _ \| '_ \ '_ \ | || |
    // |_/_/ \_\ .__/ .__/_|\_, |
    //         |_|  |_|     |__/
    /**
     * @brief Mixin the `fApply` method
     */
    class MixinFApply : public Static_curried<MixinFApply, Nat<1>>
    {
      template<HasFMapAndFlatMap Base>
      class Result : public Base
      {
        class FApply : public Static_curried<FApply, Nat<2>>
        {
        public:
          template<typename F, typename T>
          static constexpr auto
          call(F&& mf, T&& mx)
          {
            return Base::flatMap([mx = forward<T>(mx)](auto f){ return Base::fMap(f, mx); },
                                 forward<F>(mf));
          }
        };

      public:
        using Base::Base;
        static constexpr FApply fApply{};
      };

    public:
      template<HasFMapAndFlatMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFApply<Result<Base>>);
      }
    } static constexpr mixinFApply{};


  public:

    template<HasMinimalMonad Base>
    static constexpr auto
    call(Type<Base>){

      if constexpr (MissingFlatMap<Base>){
        return call(mixinFlatMap(type<Base>));

      } else if constexpr (MissingFMap<Base>){
        return call(mixinFMap(type<Base>));

      } else if constexpr (MissingFlatten<Base>) {
        return call(mixinFlatten(type<Base>));

      } else if constexpr (MissingFApply<Base>)  {
        return call(mixinFApply(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadCore<Base>);
        static_assert(HasApplicativeCore<Base>);

      }
    }
  } constexpr mixinMonadCore{};



  class MixinMonadUtility : public Static_curried<MixinMonadUtility, Nat<1>>{

    //  ___     _                 __  __
    // | _ \___| |_ _  _ _ _ _ _ |  \/  |
    // |   / -_)  _| || | '_| ' \| |\/| |
    // |_|_\___|\__|\_,_|_| |_||_|_|  |_|
    class MixinReturnM : public Static_curried<MixinReturnM, Nat<1>>
    {
      template<HasPure Base>
      class Result : public Base
      {
      public:
        static constexpr auto returnM = Base::pure;
      };

    public:
      template<HasPure Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasReturnM<Result<Base>>);
      }
    } static constexpr mixinReturnM{};



    //  _        _   __  __
    // | |   ___| |_|  \/  |
    // | |__/ -_)  _| |\/| |
    // |____\___|\__|_|  |_|

    class MixinLetM : public Static_curried<MixinLetM, Nat<1>>
    {
      template<HasFlatMap Base>
      class Result : public Base
      {
        class LetM : public Static_curried<LetM, Nat<2>>
        {
          public:
          template<typename T, typename F>
          static constexpr auto
          call(T&& mx, F&& f){
            return Base::flatMap(forward<F>(f), forward<T>(mx));
          }
        };

      public:
        using Base::Base;
        static constexpr LetM letM{};
      };

      public:
      template<HasFlatMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLetM<Result<Base>>);
      }
    } static constexpr mixinLetM{};


    //  ___           _      __  __
    // | _ ) ___ __ _(_)_ _ |  \/  |
    // | _ \/ -_) _` | | ' \| |\/| |
    // |___/\___\__, |_|_||_|_|  |_|
    //          |___/
    class MixinBeginM : public Static_curried<MixinBeginM, Nat<1>>
    {
      template<HasFlatMap Base>
      class Result : public Base
      {
        class BeginM : public Static_curried<BeginM, Nat<2>>
        {
          public:

          template<typename T, typename ... Ts>
          static constexpr auto
          call( T&& mx, Ts&& ... mxs){
            if constexpr (count_types<Ts...>() == 0){ return mx; }
            else { return Base::flatMap([=](auto){ return call(mxs ...); }, mx); }
          }
        };

        public:
        using Base::Base;
        static constexpr BeginM beginM{};
      };

      public:
      template<HasFlatMap Base>
      static constexpr auto call(Type<Base>)
      {
          return type<Result<Base>>;
          static_assert(HasBeginM<Result<Base>>);
      }
    } static constexpr mixinBeginM{};


  public:

    template<HasMonadCore Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingApplicativeUtility<Base>) {
        return call(mixinApplicativeUtility(type<Base>));

      } else if constexpr (MissingReturnM<Base>) {
        return call(mixinReturnM(type<Base>));

      } else if constexpr (MissingLetM<Base>) {
        return call(mixinLetM(type<Base>));

      } else if constexpr (MissingBeginM<Base>) {
        return call(mixinBeginM(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadUtility<Base>);
      }
    }
  } constexpr mixinMonadUtility{};


  class MixinMonad : public Static_curried<MixinMonad, Nat<1>>{
  public:
    template<HasMinimalMonad Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadCore<Base>){
        return call(mixinMonadCore(type<Base>));
      } else if constexpr (MissingMonadUtility<Base>){
        return call(mixinMonadUtility(type<Base>));
      } else {
        return type<Base>;
        static_assert(HasMonad<Base>);
      }
    }
  } constexpr mixinMonad{};


  class ProtoMonad : public Applicative
  {

    class FlatMap : public Static_curried<FlatMap, Nat<2>>{
      static constexpr auto askFlatMap =
        asksC3([]<typename Context, typename F, typename T>
               (Context, F&& f, T&& mx){
                 return Context::flatMap(forward<F>(f), forward<T>(mx));
               });
    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx){
        return
          letC(askExpel, [=](auto expel){ return
          letC(askFlatMap, [=](auto flatMap){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(flatMap([=](auto x){ return expel(f(x)); }, mx)); }); }); });
      }
    }; // end of class FlatMap

    class Flatten : public Static_curried<Flatten, Nat<1>>{
      static constexpr auto askFlatten =
        asksC2([]<typename Context, typename T>
               (Context, T&& mmx){
                 return Context::flatten(forward<T>(mmx)); });
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmcmx){
        return
          letC(askExpel, [=](auto expel){ return
          letC(askFlatten, [=](auto flatten){ return
          letC(fMap(expel, digest(cmcmx)), [=](auto mmx){ return
                returnC(flatten(mmx)); }); }); });
      }
    }; // end of class Flatten

  public:


    static constexpr FlatMap flatMap{};
    static constexpr Flatten flatten{};
  }; // end of class ProtoMonad

  class Monad : public Derive<ProtoMonad, MixinMonad>
  {
  public:

    // TODO Remove the following class for deriving functions
    template<typename Base>
    class MonadicFMap {
      class FMap : public Static_curried<FMap, Nat<2>>{
      public:
        template<typename F, typename T>
        static constexpr auto
        call(F&& f, T&& mx){
          return Base::flatMap(compose([](auto x){return Base::pure(x); }, forward<F>(f)), mx);
        }
      };
    public:
      static constexpr FMap fMap{};
    };

    template<typename Base>
    class MonadicFApply {
     class FApply : public Static_curried<FApply, Nat<2>>{
     public:
       template<typename F, typename T>
       static constexpr auto
       call(F&& mf, T&& mx){
         return Base::flatMap(
           [mx = forward<T>(mx)](auto&& f){return Base::fMap(f, mx); },
           mf);
       }
     };
    public:
      static constexpr FApply fApply{};
    };

    template<typename Base>
    class MonadicFlatMap {
      class FlatMap : public Static_curried<FlatMap, Nat<2>>{
      public:
        template<typename F, typename T>
        static constexpr auto
        call(F&& f, T&& mx){
          return Base::flatten(Base::fMap(forward<F>(f), forward<T>(mx)));
        }
      };
    public:
      static constexpr FlatMap flatMap{};
    };

    template<typename Base>
    class MonadicFlatten  {
      class Flatten : public Static_curried<Flatten, Nat<1>>{
      public:
        template<typename T>
        static constexpr auto
        call(T&& mmx){
          return Base::flatMap(identity, mmx);
        }
      };
    public:
      static constexpr Flatten flatten{};
    };
  };

  static_assert(HasFunctorCore<Monad>);
  static_assert(HasFunctorUtility<Monad>);
  static_assert(HasApplicativeCore<Monad>);
  static_assert(HasApplicativeUtility<Monad>);
  static_assert(HasMonadCore<Monad>);
  static_assert(HasMonadUtility<Monad>);

} // end of namespace Contextual::Details
