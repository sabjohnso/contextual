//
// ... Standard header files
//
#include <utility>
#include <string>
#include <type_traits>


//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... External header files
//
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Monad.hpp>

using std::pair;
using std::string;
using std::to_string;

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::declval;
    using std::decay_t;
    using std::result_of_t;

    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::ListType;
    using ListProcessing::Dynamic::list;

    constexpr auto add = curry([](auto x, auto y){ return x+y; }, nat<2>);
    constexpr auto twc = [](auto x){ return x+x; };

    constexpr auto fApply  = Monad::fApply;
    constexpr auto fMap    = Monad::fMap;
    constexpr auto pure    = Monad::pure;
    constexpr auto returnM = Monad::returnM;
    constexpr auto flatten = Monad::flatten;
    constexpr auto flatMap = Monad::flatMap;
    constexpr auto letM    = Monad::letM;



    class ProtoIdentityMonad
    {
    public:
      static constexpr auto fMap = [](auto f, auto mx){ return f(mx); };
      static constexpr auto pure = [](auto x){ return x; };
      static constexpr auto flatten = [](auto mmx){ return mmx; };
    };

    class IdentityMonad : public Derive<ProtoIdentityMonad, MixinMonad>
    {} constexpr identityMonad{};


    class ProtoFunctionMonad
    {
    public:
      static constexpr auto pure = [](auto x){ return [=](auto){ return x; };; };
      static constexpr auto flatMap = [](auto f, auto mx){ return [=](auto e){ return f(mx(e))(e); }; };
    };

    class FunctionMonad : public Derive<ProtoFunctionMonad, MixinMonad>
    {} constexpr functionMonad{};


    class ProtoListMonad
    {
      class FlatMap: public Static_curried<FlatMap, Nat<2>> {
      public:
        template<typename F>
        static Nil
        call(F f, Nil){
          return Nil{};
        }

        template<typename F, typename T, typename R = result_of_t<F(typename T::value_type)>>
        static R
        call(F f, T xs){
          return hasData(xs)
            ? append(f(head(xs)), call(f, tail(xs)))
            : R::nil;
        }
      };
    public:
      static constexpr auto pure = [](auto x){ return list(x); };
      static constexpr FlatMap flatMap{};

      // static constexpr auto fMap = [](auto f, auto xs){ return map(f, xs); };
    };

    class ListMonad : public Derive<ProtoListMonad, MixinMonad>
    {} constexpr listMonad{};
  } // end of anonymous namespace

  TEST(Monad, ReturnIdentity){
    EXPECT_EQ(
      run(identityMonad)(
        returnM(3)),
      3);
  }

  TEST(Monad, FlatMapIdentity){
    EXPECT_EQ(
      run(identityMonad)(
        flatMap([](auto x){ return x+x; }, pure(3))),
      6);
  }

  TEST(Monad, LetMIdentity){
    EXPECT_EQ(
      run(identityMonad)(
        letM(pure(3), [](auto x){ return
              returnM(x+x);
        })),
      6);
  }

  TEST(Monad, FlattenIdentity0){
    EXPECT_EQ(run(identityMonad)( flatten(3)), 3);
  }

  TEST(Monad, FlattenIdentity1){
    EXPECT_EQ(run(identityMonad)( flatten(pure(3))), 3);
  }

  TEST(Monad, FlattenIdentity2){
    EXPECT_EQ(run(identityMonad)( flatten(pure(pure(3)))), 3);
  }


  TEST(Monad, ReturnFunctionMonad){
    EXPECT_EQ(
      run("x")(
        run(functionMonad)(
          returnM(3))),
      3);
  }

  TEST(Monad, flatMapFunctionMonad){
    EXPECT_EQ(
      run(4)(
        run(functionMonad)(
          flatMap(add, constant(3)))),
      7);
  }

  TEST(Monad, LetMFunctionMonad){
    EXPECT_EQ(
      run(4)(
        run(functionMonad)(
          letM(add(3),[](auto x){ return
                constant(to_string(x));
          }))),
      string("7"));
  }

  TEST(Monad, LetMFunctionMonad2){
    EXPECT_EQ(
      run(4)(
        run(functionMonad)(
          letM(add(2), [=](auto x){ return
          letM(add(3), [=](auto y){ return
                returnM(x + y); }); }))),
      13);
  }


  TEST(Monad, ListPure){
    EXPECT_EQ(run(listMonad,pure(3)), list(3));
  }

  TEST(Monad, ListReturn){
    EXPECT_EQ(run(listMonad, returnM(3)), list(3));
  }

  TEST(Monad, ListFMap){
    EXPECT_EQ(
      run(listMonad)(fMap(twc, list(1, 2))),
      list(2, 4));
  }

  TEST(Monad, ListFApply){
    EXPECT_EQ(
      run(listMonad)(
        fApply(fMap(add, list(1, 2)),
               list(3, 4))),
      list(4, 5, 5, 6));
  }

  TEST(Monad, ListFlatMap){
    EXPECT_EQ(
      run(listMonad)(
        flatMap([](auto x){ return list(x, x); },
                list(1, 2))),
      list(1, 1, 2, 2));
  }

  TEST(Monad, ListLetM){
    EXPECT_EQ(
      run(listMonad)(
        letM(list(1, 2), [=](auto x){ return
        letM(list(3, 4), [=](auto y){ return
              returnM(x+y); }); })),
      list(4, 5, 5, 6));
  }

  TEST(Monad, ListFlatten){
    EXPECT_EQ(
      run(listMonad, flatten(list(list(1,2), list(3, 4)))),
      list(1, 2, 3, 4));
  }

} // end of namespace Contextual::Details::Testing
