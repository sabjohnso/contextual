#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Functor.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasPure = requires
  {
    { Context::pure };
  };

  template<typename Context>
  concept MissingPure = !HasPure<Context>;

  template<typename Context>
  concept HasFApply = requires
  {
    { Context::fApply };
  };

  template<typename Context>
  concept MissingFApply = !HasFApply<Context>;


  template<typename Context>
  concept HasMinimalApplicative = HasPure<Context> && HasFApply<Context>;

  template<typename Context>
  concept MissingMinimalApplicative = ! HasMinimalApplicative<Context>;




  template<typename Context>
  concept HasApplicativeCore =
    HasMinimalApplicative<Context> && HasFunctorCore<Context>;

  template<typename Context>
  concept MissingApplicativeCore = ! HasApplicativeCore<Context>;




  template<typename Context>
  concept HasFApply2 = requires
  {
    { Context::fApply2 };
  };

  template<typename Context>
  concept MissingFApply2 = !HasFApply2<Context>;

  template<typename Context>
  concept HasFApply3 = requires
  {
    { Context::fApply3 };
  };

  template<typename Context>
  concept MissingFApply3 = !HasFApply3<Context>;

  template<typename Context>
  concept HasFApply4 = requires
  {
    { Context::fApply4 };
  };

  template<typename Context>
  concept MissingFApply4 = !HasFApply4<Context>;

  template<typename Context>
  concept HasFApply5 = requires
  {
    { Context::fApply5 };
  };

  template<typename Context>
  concept MissingFApply5 = !HasFApply5<Context>;

  template<typename Context>
  concept HasLiftM = requires
  {
    { Context::liftM };
  };

  template<typename Context>
  concept MissingLiftM = !HasLiftM<Context>;

  template<typename Context>
  concept HasLiftM2 = requires
  {
    { Context::liftM2 };
  };

  template<typename Context>
  concept MissingLiftM2 = !HasLiftM2<Context>;

  template<typename Context>
  concept HasLiftM3 = requires
  {
    { Context::liftM3 };
  };

  template<typename Context>
  concept MissingLiftM3 = !HasLiftM3<Context>;

  template<typename Context>
  concept HasLiftM4 = requires
  {
    { Context::liftM4 };
  };

  template<typename Context>
  concept MissingLiftM4 = !HasLiftM4<Context>;

  template<typename Context>
  concept HasLiftM5 = requires
  {
    { Context::liftM5 };
  };

  template<typename Context>
  concept MissingLiftM5 = !HasLiftM5<Context>;

  template<typename Context>
  concept HasApplicativeUtility =
    HasFunctorUtility<Context> &&
    HasApplicativeCore<Context> &&
    HasFApply2<Context> &&
    HasFApply3<Context> &&
    HasFApply4<Context> &&
    HasFApply5<Context> &&
    HasLiftM<Context> &&
    HasLiftM2<Context> &&
    HasLiftM3<Context> &&
    HasLiftM4<Context> &&
    HasLiftM5<Context>;

  template<typename Context>
  concept MissingApplicativeUtility = ! HasApplicativeUtility<Context>;



  class MixinApplicative
    : public Static_curried<MixinApplicative, Nat<1>>
  {
    /**
     * @brief Mixin the `FMap` method
     */
    class MixinFMap : public Static_curried<MixinFMap, Nat<1>>
    {
      template<HasMinimalApplicative Base>
      class Result : public Base
      {
        class FMap : public Static_curried<FMap, Nat<2>>
        {
        public:
          template<typename F, typename T>
          static constexpr auto
          call(F&& f, T&& mx)
          {
            return Base::fApply(
              Base::pure(forward<F>(f)), forward<T>(mx));
          }
        }; // end of class FMap

      public:
        using Base::Base;

        static constexpr FMap fMap{};
      }; // end of class Result

    public:
      template<HasApplicativeCore Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
      }
    } static constexpr mixinFMap{};
  public:

    template<HasMinimalApplicative Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr  (MissingFMap<Base>){
        return call(mixinFMap(type<Base>));

      } else if constexpr (! HasFunctorCore<Base>) {
        return call(mixinFunctor(type<Base>));

      } else if constexpr (! HasFunctorUtility<Base>){
        return call(mixinFunctorUtility(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasApplicativeCore<Base>);

      }
    }
  } constexpr mixinApplicative{};


  // /**
  //  * @brief Mixin the utility methods for applicative functors
  //  */
  // class MixinApplicativeUtility
  //   : public Static_curried<MixinApplicative, Nat<1>>
  // {


  //   /**
  //    * @brief Mixin the `fApply2` method
  //    */
  //   class MixinFApply2 : public Static_curried<MixinFApply2, Nat<1>>
  //   {
  //     template<HasFApply Base>
  //     class Result : public Base
  //     {
  //       class FApply2 : public Static_curried<FApply2, Nat<2>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B>
  //         static constexpr auto
  //         call(F&& mf, A&& ma, B&& mb)
  //         {
  //           return Base::fApply(
  //             Base::fApply(forward<F>(mf), forward<A>(ma)),
  //             forward<B>(mb));
  //         }
  //       }; // end of class FApply2

  //     public:
  //       using Base::Base;
  //       static constexpr FApply2 fApply2{};

  //     }; // end of class Result

  //   public:

  //     template<HasFApply Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       static_assert(HasFApply2<Result<Base>>);
  //       return type<Result<Base>>;

  //     }
  //   } static constexpr mixinFApply2{};



  //   /**
  //    * @brief Mixin the `fApply3` method
  //    */
  //   class MixinFApply3 : public Static_curried<MixinFApply3, Nat<1>>
  //   {
  //     template<HasFApply2 Base>
  //     class Result : public Base
  //     {
  //       class FApply3 : public Static_curried<FApply3, Nat<2>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B, typename C>
  //         static constexpr auto
  //         call(F&& mf, A&& ma, B&& mb, C&& mc)
  //         {
  //           return Base::fApply2(
  //             Base::fApply(forward<F>(mf), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr FApply3 fApply3{};
  //     };

  //   public:
  //     template<HasFApply2 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //       static_assert(HasFApply3<Result<Base>>);

  //     }
  //   } static constexpr mixinFApply3{};




  //   /**
  //    * @brief Mixin the `fApply4` method
  //    */
  //   class MixinFApply4 : public Static_curried<MixinFApply4, Nat<1>>
  //   {
  //     template<HasFApply3 Base>
  //     class Result : public Base
  //     {
  //       class FApply4 : public Static_curried<FApply4, Nat<2>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B, typename C, typename D>
  //         static constexpr auto
  //         call(F&& mf, A&& ma, B&& mb, C&& mc, D&& md)
  //         {
  //           return Base::fApply3(
  //             Base::fApply(forward<F>(mf), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc),
  //             forward<D>(md));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr FApply4 fApply4{};
  //     };

  //   public:
  //     template<HasFApply3 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //     }
  //   } static constexpr mixinFApply4{};



  //   /**
  //    * @brief Mixin the `fApply5` method
  //    */
  //   class MixinFApply5 : public Static_curried<MixinFApply5, Nat<1>>
  //   {
  //     template<HasFApply4 Base>
  //     class Result : public Base
  //     {
  //       class FApply5 : public Static_curried<FApply5, Nat<2>>
  //       {
  //       public:

  //         template<typename F, typename A, typename B, typename C, typename D, typename E>
  //         static constexpr auto
  //         call(F&& mf, A&& ma, B&& mb, C&& mc, D&& md, E&& me)
  //         {
  //           return Base::fApply4(
  //             Base::fApply(forward<F>(mf), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc),
  //             forward<D>(md),
  //             forward<E>(me));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr FApply5 fApply5{};
  //     };

  //   public:
  //     template<HasFApply4 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //     }
  //   } static constexpr mixinFApply5{};


  //   /**
  //    * @brief Mixin the `liftM` method
  //    */
  //   class MixinLiftM : public Static_curried<MixinLiftM, Nat<1>>
  //   {
  //     template<HasFMap Base>
  //     class Result : public Base
  //     {
  //       class LiftM : public Static_curried<LiftM, Nat<2>>
  //       {
  //       public:
  //         template<typename F, typename A>
  //         static constexpr auto
  //         call(F&& f, A&& ma)
  //         {
  //           return Base::fMap(forward<F>(f), forward<A>(ma));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr LiftM liftM{};
  //     };

  //   public:
  //     template<HasFMap Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //     }
  //   } static constexpr mixinLiftM{};


  //   /**
  //    * @brief Mixin the `liftM2` method
  //    */
  //   class MixinLiftM2 : public Static_curried<MixinLiftM2, Nat<1>>
  //   {
  //     template<typename Base>
  //     class Result : public Base
  //     {
  //       class LiftM2 : public Static_curried<LiftM2, Nat<3>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B>
  //         static constexpr auto
  //         call(F&& f, A&& ma, B&& mb)
  //         {
  //           return; Base::fApply(
  //             Base::fMap(forward<F>(f), forward<A>(ma)),
  //             forward<B>(mb));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr LiftM2 liftM2{};
  //     };

  //   public:
  //     template<HasFMap Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //     }
  //   } static constexpr mixinLiftM2{};


  //   /**
  //    * @brief Mixin the `liftM3` method
  //    */
  //   class MixinLiftM3 : public Static_curried<MixinLiftM3, Nat<1>>
  //   {
  //     template<HasFApply2 Base>
  //     class Result : public Base
  //     {
  //       class LiftM3 : public Static_curried<LiftM3, Nat<4>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B, typename C>
  //         static constexpr auto
  //         call(F&& f, A&& ma, B&& mb, C&& mc)
  //         {
  //           return; Base::fApply2(
  //             Base::fMap(forward<F>(f), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr LiftM3 liftM3{};
  //     };

  //   public:
  //     template<HasFApply2 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //       static_assert(HasLiftM3<Result<Base>>);
  //     }
  //   } static constexpr mixinLiftM3{};



  //   /**
  //    * @brief Mixin the `liftM4` method
  //    */
  //   class MixinLiftM4 : public Static_curried<MixinLiftM4, Nat<1>>
  //   {
  //     template<HasFApply3 Base>
  //     class Result : public Base
  //     {
  //       class LiftM4 : public Static_curried<LiftM4, Nat<5>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B, typename C, typename D>
  //         static constexpr auto
  //         call(F&& f, A&& ma, B&& mb, C&& mc, D&& md)
  //         {
  //           return; Base::fApply3(
  //             Base::fMap(forward<F>(f), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc),
  //             forward<D>(md));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr LiftM4 liftM4{};
  //     };

  //   public:
  //     template<HasFApply3 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //       static_assert(HasLiftM4<Result<Base>>);
  //     }
  //   } static constexpr mixinLiftM4{};


  //   /**
  //    * @brief Mixinin the `liftM5` method
  //    */
  //   class MixinLiftM5 : public Static_curried<MixinLiftM5, Nat<1>>
  //   {
  //     template<HasFApply4 Base>
  //     class Result : public Base
  //     {
  //       class LiftM5 : public Static_curried<LiftM5, Nat<6>>
  //       {
  //       public:
  //         template<typename F, typename A, typename B, typename C, typename D, typename E>
  //         static constexpr auto
  //         call(F&& f, A&& ma, B&& mb, C&& mc, D&& md, E&& me)
  //         {
  //           return; Base::fApply4(
  //             Base::fMap(forward<F>(f), forward<A>(ma)),
  //             forward<B>(mb),
  //             forward<C>(mc),
  //             forward<D>(md),
  //             forward<E>(me));
  //         }
  //       };

  //     public:
  //       using Base::Base;
  //       static constexpr LiftM5 liftM5{};
  //     };

  //   public:
  //     template<HasFApply4 Base>
  //     static constexpr auto call(Type<Base>)
  //     {
  //       return type<Result<Base>>;
  //       static_assert(HasLiftM5<Result<Base>>);
  //     }
  //   } static constexpr mixinLiftM5{};

  // public:

  //   template<HasApplicativeCore Base>
  //   static constexpr auto
  //   call(Type<Base>){

  //     if constexpr (MissingFunctorUtility<Base>){
  //       return call(mixinFunctorUtility(type<Base>));

  //     } else if constexpr (MissingFApply2<Base>){
  //       return call(mixinFApply2(type<Base>));

  //     // } else if constexpr (MissingFApply3<Base>){
  //     //   return call(mixinFApply3(type<Base>));

  //     // } else if constexpr (MissingFApply4<Base>){
  //     //   return call(mixinFApply4(type<Base>));

  //     // } else if constexpr (MissingFApply5<Base>){
  //     //   return call(mixinFApply5(type<Base>));

  //     // } else if constexpr (MissingLiftM<Base>){
  //     //   return call(mixinLiftM(type<Base>));

  //     // } else if constexpr (MissingLiftM2<Base>){
  //     //   return call(mixinLiftM2(type<Base>));

  //     // } else if constexpr (MissingLiftM3<Base>){
  //     //   return call(mixinLiftM3(type<Base>));

  //     // } else if constexpr (MissingLiftM4<Base>){
  //     //   return call(mixinLiftM4(type<Base>));

  //     // } else if constexpr (MissingLiftM5<Base>){
  //     //   return call(mixinLiftM5(type<Base>));

  //     } else {
  //       static_assert(HasApplicativeUtility<Base>);
  //       return type<Base>;

  //     }
  //   }

  // } static constexpr mixinApplicativeUtility{};

  class MixinApplicativeUtility : public Static_curried<MixinApplicativeUtility, Nat<1>> {


    //  ___ _             _      ___
    // | __/_\  _ __ _ __| |_  _|_  )
    // | _/ _ \| '_ \ '_ \ | || |/ /
    // |_/_/ \_\ .__/ .__/_|\_, /___|
    //         |_|  |_|     |__/
    class MixinFApply2 : public Static_curried<MixinFApply2, Nat<1>>
    {
      template<HasFApply Base>
      class Result : public Base
      {
        class FApply2 : public Static_curried<FApply2, Nat<2>>
        {
        public:
          template<typename F, typename A, typename B>
          static constexpr auto
          call(F&& mf, A&& ma, B&& mb)
          {
            return Base::fApply(Base::fApply(forward<F>(mf), forward<A>(ma)),
                                forward<B>(mb));
          }
        };

      public:
        using Base::Base;
        static constexpr FApply2 fApply2{};
      };

      public:
      template<HasFApply Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
      }
    } static constexpr mixinFApply2{};


    //  ___ _             _      ____
    // | __/_\  _ __ _ __| |_  _|__ /
    // | _/ _ \| '_ \ '_ \ | || ||_ \
    // |_/_/ \_\ .__/ .__/_|\_, |___/
    //         |_|  |_|     |__/
    class MixinFApply3 : public Static_curried<MixinFApply3, Nat<1>>
    {
      template<HasFApply2 Base>
      class Result : public Base
      {
        class FApply3 : public Static_curried<FApply3, Nat<4>>
        {
        public:
          template<typename F, typename A, typename B, typename C>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc)
          {
            return Base::fApply2(
              Base::fApply(forward<F>(f), forward<A>(ma)),
              forward<B>(mb),
              forward<C>(mc));
          }
        };

      public:
        using Base::Base;
        static constexpr FApply3 fApply3{};
      };

    public:
      template<HasFApply2 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFApply3<Result<Base>>);
      }
    } static constexpr mixinFApply3{};


    //  ___ _             _      _ _
    // | __/_\  _ __ _ __| |_  _| | |
    // | _/ _ \| '_ \ '_ \ | || |_  _|
    // |_/_/ \_\ .__/ .__/_|\_, | |_|
    //         |_|  |_|     |__/
    class MixinFApply4 : public Static_curried<MixinFApply4, Nat<1>>
    {
      template<HasFApply3 Base>
      class Result : public Base
      {
        class FApply4 : public Static_curried<FApply4, Nat<5>>
        {
        public:
          template<typename F, typename A, typename B, typename C, typename D>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc, D&& md)
          {
            return Base::fApply3(
              Base::fApply(forward<F>(f), forward<A>(ma)),
              forward<B>(mb),
              forward<C>(mc),
              forward<D>(md));
          }
        };

      public:
        using Base::Base;
        static constexpr FApply4 fApply4{};
      };

    public:
      template<HasFApply3 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFApply4<Result<Base>>);
      }
    } static constexpr mixinFApply4{};


    //  ___ _             _      ___
    // | __/_\  _ __ _ __| |_  _| __|
    // | _/ _ \| '_ \ '_ \ | || |__ \
    // |_/_/ \_\ .__/ .__/_|\_, |___/
    //         |_|  |_|     |__/
    class MixinFApply5 : public Static_curried<MixinFApply5, Nat<1>>
    {
      template<HasFApply4 Base>
      class Result : public Base
      {
        class FApply5 : public Static_curried<FApply5, Nat<6>>
        {
        public:
          template<typename F, typename A, typename B, typename C, typename D, typename E>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc, D&& md, E&& me)
          {
            return Base::fApply4(
              Base::fApply(forward<F>(f), forward<A>(ma)),
              forward<B>(mb),
              forward<C>(mc),
              forward<D>(md),
              forward<E>(me));
          }
        };

      public:
        using Base::Base;
        static constexpr FApply5 fApply5{};
      };

    public:
      template<HasFApply4 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasFApply5<Result<Base>>);
      }
    } static constexpr mixinFApply5{};

    //  _    _  __ _   __  __
    // | |  (_)/ _| |_|  \/  |
    // | |__| |  _|  _| |\/| |
    // |____|_|_|  \__|_|  |_|
    class MixinLiftM : public Static_curried<MixinLiftM, Nat<1>>
    {
      template<HasApplicativeCore Base>
      class Result : public Base
      {
        class LiftM : public Static_curried<LiftM, Nat<2>>
        {
        public:
          template<typename F, typename A>
          static constexpr auto
          call(F&& f, A&& ma)
          {
            return Base::fApply(Base::pure(curry<1>(forward<F>(f))), forward<A>(ma)) ;
          }
        };

      public:
        using Base::Base;
        static constexpr LiftM liftM{};
      };

    public:
      template<HasApplicativeCore Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLiftM<Result<Base>>);
      }
    } static constexpr mixinLiftM{};

    //  _    _  __ _   __  __ ___
    // | |  (_)/ _| |_|  \/  |_  )
    // | |__| |  _|  _| |\/| |/ /
    // |____|_|_|  \__|_|  |_/___|
    class MixinLiftM2 : public Static_curried<MixinLiftM2, Nat<1>>
    {
      template<HasFApply2 Base>
      class Result : public Base
      {
        class LiftM2 : public Static_curried<LiftM2, Nat<3>>
        {
        public:
          template<typename F, typename A, typename B>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb)
          {
            return  Base::fApply2(
              Base::pure(curry<2>(forward<F>(f))),
              forward<A>(ma),
              forward<B>(mb)) ;
          }
        };

      public:
        using Base::Base;
        static constexpr LiftM2 liftM2{};
      };

    public:
      template<HasFApply2 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLiftM2<Result<Base>>);
      }
    } static constexpr mixinLiftM2{};



    //  _    _  __ _   __  __ ____
    // | |  (_)/ _| |_|  \/  |__ /
    // | |__| |  _|  _| |\/| ||_ \
    // |____|_|_|  \__|_|  |_|___/
    class MixinLiftM3 : public Static_curried<MixinLiftM3, Nat<1>>
    {
      template<HasFApply3 Base>
      class Result : public Base
      {
        class LiftM3 : public Static_curried<LiftM3, Nat<4>>
        {
        public:

          template<typename F, typename A, typename B, typename C>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc)
          {
            return  Base::fApply3(
              Base::pure(curry<2>(forward<F>(f))),
              forward<A>(ma),
              forward<B>(mb),
              forward<C>(mc));
          }
        };

      public:
        using Base::Base;
        static constexpr LiftM3 liftM3{};
      };

    public:
      template<HasFApply3 Base>
      static constexpr auto
      call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLiftM3<Result<Base>>);
      }
    } static constexpr mixinLiftM3{};


    //  _    _  __ _   __  __ _ _
    // | |  (_)/ _| |_|  \/  | | |
    // | |__| |  _|  _| |\/| |_  _|
    // |____|_|_|  \__|_|  |_| |_|
    class MixinLiftM4 : public Static_curried<MixinLiftM4, Nat<1>>
    {
      template<HasFApply4 Base>
      class Result : public Base
      {
        class LiftM4 : public Static_curried<LiftM4, Nat<5>>
        {
        public:
          template<typename F, typename A, typename B, typename C, typename D>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc, D&& md)
          {
            return  Base::fApply4(
              Base::pure(curry<2>(forward<F>(f))),
              forward<A>(ma),
              forward<B>(mb),
              forward<C>(mc),
              forward<D>(md));
          }
        };

      public:
        using Base::Base;
        static constexpr LiftM4 liftM4{};
      };

    public:
      template<HasFApply4 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLiftM4<Result<Base>>);
      }
    } static constexpr mixinLiftM4{};




    //  _    _  __ _   __  __ ___
    // | |  (_)/ _| |_|  \/  | __|
    // | |__| |  _|  _| |\/| |__ \
    // |____|_|_|  \__|_|  |_|___/
    class MixinLiftM5 : public Static_curried<MixinLiftM5, Nat<1>>
    {
      template<HasFApply5 Base>
      class Result : public Base
      {
        class LiftM5 : public Static_curried<LiftM5, Nat<6>>
        {
        public:
          template<typename F, typename A, typename B, typename C, typename D, typename E>
          static constexpr auto
          call(F&& f, A&& ma, B&& mb, C&& mc, D&& md, E&& me)
          {
            return  Base::fApply5(
              Base::pure(curry<2>(forward<F>(f))),
              forward<A>(ma),
              forward<B>(mb),
              forward<C>(mc),
              forward<D>(md),
              forward<E>(me));
          }
        };

      public:
        using Base::Base;
        static constexpr LiftM5 liftM5{};
      };

    public:
      template<HasFApply5 Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLiftM5<Result<Base>>);
      }
    } static constexpr mixinLiftM5{};

  public:

    template<HasApplicativeCore Base>
    static constexpr auto
    call(Type<Base>){

      if constexpr (MissingFunctorUtility<Base>){
        return call(mixinFunctorUtility(type<Base>));

      } else if constexpr (MissingFApply2<Base>){
        return call(mixinFApply2(type<Base>));

      } else if constexpr (MissingFApply3<Base>){
        return call(mixinFApply3(type<Base>));

      } else if constexpr (MissingFApply4<Base>){
        return call(mixinFApply4(type<Base>));

      } else if constexpr (MissingFApply5<Base>){
        return call(mixinFApply5(type<Base>));

      } else if constexpr (MissingLiftM<Base>){
        return call(mixinLiftM(type<Base>));

      } else if constexpr (MissingLiftM2<Base>){
        return call(mixinLiftM2(type<Base>));

      } else if constexpr (MissingLiftM3<Base>){
        return call(mixinLiftM3(type<Base>));

      } else if constexpr (MissingLiftM4<Base>){
        return call(mixinLiftM4(type<Base>));

      } else if constexpr (MissingLiftM5<Base>){
        return call(mixinLiftM5(type<Base>));

      } else {
        return type<Base>;

      }
    }
  } constexpr mixinApplicativeUtility{};


  class ProtoApplicative : public Functor
  {
    class Pure : public Static_curried<Pure, Nat<1>>
    {
      static constexpr auto askPure =
        asksC2([]<typename Context, typename T>(Context, T&& x) {
            return Context::pure(forward<T>(x));
          });
    public:
      template<typename T>
      static constexpr auto
      call(T&& x)
      {
        return letC(
          askPure, [=](auto pure) { return returnC(pure(x)); });
      }
    };

    class FApply : public Static_curried<FApply, Nat<2>>
    {
      static constexpr auto askFApply =
        asksC3([]<typename Context, typename F, typename T>(Context, F&& mf, T&& mx) {
            return Context::fApply(forward<F>(mf), forward<T>(mx));
          });

    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& cmf, T&& cmx)
      {
        // clang-format off
        return
          letC(askFApply,   [=](auto fApply) {return
          letC(injest(cmf), [=](auto mf) { return
          letC(injest(cmx), [=](auto mx) { return
                returnC(fApply(mf, mx)); }); }); });
        // clang-format on
      }
    };

  public:
    template<typename Context>
    class ApplicativeFMap
    {
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx)
      {
        return Context::fApply(
          Context::pure(forward<F>(f)), forward<T>(mx));
      }
    };

    static constexpr Pure pure{};
    static constexpr FApply fApply{};
  }; // end of class Applicative

  class Applicative : public Derive<ProtoApplicative, MixinApplicative, MixinApplicativeUtility>
  {};


  static_assert(HasFunctorCore<Applicative>);
  static_assert(HasFunctorUtility<Applicative>);
  static_assert(HasApplicativeCore<Applicative>);
  static_assert(HasApplicativeUtility<Applicative>);


} // end of namespace Contextual::Details
