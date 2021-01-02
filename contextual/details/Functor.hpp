#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Derive.hpp>
#include <contextual/details/Constant.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/import.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasFMap = requires
  {
    { Context::fMap };
  };

  template<typename Context>
  concept MissingFMap = !HasFMap<Context>;

  template<typename Context>
  concept HasMinimalFunctor = HasFMap<Context>;

  template<typename Context>
  concept MissingMinimalFunctor = ! HasMinimalFunctor<Context>;

  template<typename Context>
  concept HasFunctorCore = HasFMap<Context>;

  template<typename Context>
  concept MissingFunctorCore = ! HasFunctorCore<Context>;

  template<typename Context>
  concept HasLetF = requires
  {
    { Context::letF };
  };

  template<typename Context>
  concept MissingLetF = !HasLetF<Context>;

  template<typename Context>
  concept HasHomogenize = requires
  {
    { Context::homogenize };
  };

  template<typename Context>
  concept MissingHomogenize = !HasHomogenize<Context>;


  template<typename Context>
  concept HasFunctorUtility =
    HasFunctorCore<Context> &&
    HasLetF<Context> &&
    HasHomogenize<Context>;

  template<typename Context>
  concept MissingFunctorUtility = ! HasFunctorUtility<Context>;

  /**
   * @brief Mixin the functor utility functions
   */
  class MixinFunctor : public Static_curried<MixinFunctor, Nat<1>>
  {
  public:

    template<HasMinimalFunctor Base>
    static constexpr auto
    call(Type<Base>){
      return type<Base>;
      static_assert(HasFunctorCore<Base>);
    }
  } constexpr mixinFunctor{};


  /**
   * @brief Derive a functor from a sufficiently complete input.
   */
  class MixinFunctorUtility : public Static_curried<MixinFunctorUtility, Nat<1>>
  {

    /**
     * @brief Given a type Mixin an fMap method
     */
    class MixinLetF : public Static_curried<MixinLetF, Nat<1>>
    {
      template<HasFMap Base>
      class Result : public Base
      {
        class LetF : public Static_curried<LetF, Nat<2>>
        {
        public:
          template<typename T, typename F>
          static constexpr auto
          call(T&& mx, F&& f)
          {
            return Base::fMap(forward<F>(f), forward<T>(mx));
          }
        };

      public:
        using Base::Base;
        static constexpr LetF letF{};
      };

    public:
      template<HasFMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasLetF<Result<Base>>);
      }
    } static constexpr mixinLetF{};


    /**
     * @brief Given a type Mixin a `homogenize` method
     */
    class MixinHomogenize
      : public Static_curried<MixinHomogenize, Nat<1>>
    {
      template<HasFMap Base>
      class Result : public Base
      {
        class Homogenize : public Static_curried<Homogenize, Nat<2>>
        {
        public:
          template<typename T, typename U>
          static constexpr auto
          call(T&& x, U&& mx)
          {
            return Base::fMap(constant(forward<T>(x)), forward<U>(mx));
          }
        };

      public:
        using Base::Base;
        static constexpr Homogenize homogenize{};
      };

    public:
      template<HasFMap Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasHomogenize<Result<Base>>);
      }
    } static constexpr mixinHomogenize{};


  public:

    template<HasFunctorCore Base>
    static constexpr auto
    call(Type<Base>)
    {
      if constexpr (MissingLetF<Base>) {
        return call(mixinLetF(type<Base>));

      } else if constexpr (MissingHomogenize<Base>) {
        return call(mixinHomogenize(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasFunctorUtility<Base>);
      }
    }
  } mixinFunctorUtility{};


  class ProtoFunctor
  {
    class FMap : public Static_curried<FMap, Nat<2>>
    {
      static constexpr auto askFMap =
        asksC3([]<typename Context, typename F, typename T>
               (Context, F&& f, T&& mx){
                 return Context::fMap(forward<F>(f), forward<T>(mx));
               });

    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx)
      {
        return letC(askFMap, [=](auto fMap) {
          return
            letC(injest(cmx), [=](auto mx) { return
                  returnC(fMap(f, mx)); });
        });
      }
    }; // end of class FMap

  public:
    static constexpr FMap fMap{};
  }; // end of class ProtoFunctor
  static_assert(HasMinimalFunctor<ProtoFunctor>);


  class Functor : public Derive<ProtoFunctor, MixinFunctor, MixinFunctorUtility>
  {};

  static_assert(HasFunctorCore<Functor>);
  static_assert(HasFunctorUtility<Functor>);



} // end of namespace Contextual::Details
