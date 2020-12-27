//
// ... Standard header files
//
#include <functional>
#include <type_traits>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... External header files
//
#include <function_utility/function_utility.hpp>
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Run.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Category.hpp>
#include <contextual/details/Monad.hpp>


namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::decay_t;

    constexpr auto list = [](auto x, auto ... xs){
      return ListProcessing::Dynamic::list(x, xs ...);
    };

    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::ListType;

    constexpr auto cat = Category::cat;
    constexpr auto compose = Category::compose;
    constexpr auto identity = Category::identity;

    template<typename F>
    class SimpleFunction
    {
      F f;
    public:
      constexpr
      SimpleFunction(F fun) : f(fun) {}

      template<typename T>
      constexpr auto
      operator ()(T x) const { return f(x); }

    }; // end of class SimpleFunction

    template<typename F>
    SimpleFunction(F f) -> SimpleFunction<F>;

    class FunctionContext
    {
    public:
      template<typename F>
      static constexpr auto
      cat(F f){ return SimpleFunction(f); }

      template<typename F, typename G>
      static constexpr auto
      compose(F f, G g){ return SimpleFunction(FunctionUtility::compose(f, g)); }

      static constexpr auto
      identity = SimpleFunction(FunctionUtility::identity);

    }; // end of class FunctionContext

    class FunctionMonad
      : public Deriving<
      FunctionMonad,
      Monad::MonadicFMap,
      Monad::MonadicFApply,
      Monad::MonadicFlatten
      >
    {
    public:
      static constexpr auto pure = curry<2>(
        [](auto x, auto){
          return x;
        });


      static constexpr auto flatMap = curry<3>(
        [](auto f, auto mx, auto e){
            return f(mx(e))(e);
        });
    };

    constexpr FunctionMonad functionMonad{};

    constexpr FunctionContext functionContext{};

    constexpr auto twc = [](auto x){ return x+x; };
    using Twc = decay_t<decltype(twc)>;

    constexpr auto sqr = [](auto x){ return x*x; };
    using Sqr = decay_t<decltype(sqr)>;

    constexpr auto add = curry<2>(std::plus{});
    constexpr auto mul = curry<2>(std::multiplies{});

    template<typename Monad>
    class Kleisli
    {
    public:
      template<typename F>
      static constexpr auto
      cat(F f){ return curry<1>(f); }

      template<typename F, typename G>
      static constexpr auto
      compose(F f, G g){ return
          curry<1>([=](auto x){ return Monad::flatMap([=](auto y){ return f(y); }, g(x)); });
      }

      static constexpr auto identity = Monad::pure;

    }; // end of class Kleisli

    template<typename Monad>
    constexpr Kleisli<Monad> kleisli{};

    class ListMonad
      : public Deriving<
      ListMonad,
      Monad::MonadicFMap,
      Monad::MonadicFApply,
      Monad::MonadicFlatten
      >
    {

      class FlatMap : public Static_curried<FlatMap,Nat<2>> {
      public:
        template<typename F>
        static constexpr auto
        call(F, Nil){ return Nil{}; }

        template<typename F, typename T, typename Result = result_of_t<F(T)>>
        static constexpr Result
        call(F f, ListType<T> xs){
          return hasData(xs)
            ? append(f(head(xs)), flatMap(f, tail(xs)))
            : Result::nil;
        }
      };

    public:
      static constexpr auto pure = list;
      static constexpr FlatMap flatMap{};


    };

  } // end of anonymous namespace



  TEST(Category, CatFunction){
    EXPECT_EQ(
      run(3)(run(functionContext)(cat(twc))),
      6);
  }

  TEST(Category, ComposeFunction){
    EXPECT_EQ(
      run(3)(run(functionContext)(compose(cat(twc), cat(sqr)))),
      18);
  }

  TEST(Category, IdentityFunction){
    EXPECT_EQ(
      run(3)(run(functionContext)(compose(cat(twc), identity))),
      run(3)(run(functionContext)(cat(twc))));
  }

  TEST(Category, IdentityComutesFunction){
    EXPECT_EQ(
      run(3)(run(functionContext)(compose(cat(twc), identity))),
      run(3)(run(functionContext)(compose(identity, cat(twc)))));
  }



  TEST(Category, CatKleisliFunction){
    EXPECT_EQ(
      run(kleisli<FunctionMonad>)(
        cat(constant))(3, 4),
      3);
  }

  TEST(Category, ComposeKleisliFunction){
    EXPECT_EQ(
      run(kleisli<FunctionMonad>)(
        compose(cat(mul), cat(add)))(
          3,5),
      40);
  }

  TEST(Category, IdentityKleisliFunction){
    EXPECT_EQ(
      run(kleisli<FunctionMonad>)(
        compose(identity, cat(add)))
      (3,5),
      8);
  }

  TEST(Category, IdentityCommutesKleisliFunction){
    EXPECT_EQ(
      run(kleisli<FunctionMonad>)(compose(identity, cat(add)))(3,5),
      run(kleisli<FunctionMonad>)(compose(cat(add), identity))(3,5));
  }

  TEST(Category, CatKleisliList){
    EXPECT_EQ(
      run(kleisli<ListMonad>,cat(list))(3),
      list(3));
  }

  TEST(Category, ComposeKleisliList){
    EXPECT_EQ(
      run(kleisli<ListMonad>)(
        compose(cat([](auto x){ return list(x, x);}),
                cat([](auto x){ return list(x, x, x); })))('x'),
      list('x','x','x','x','x','x'));
  }

  TEST(Category,  IdentityKleisliList){
    EXPECT_EQ(
      run(kleisli<ListMonad>)(identity)('x'),
      list('x'));
  }

  TEST(Category, ComposeIdentityKleisliList){
    EXPECT_EQ(
      run(kleisli<ListMonad>)(
        compose(cat([](auto x){ return list( x, x); }), identity))('x'),
      list('x', 'x'));
  }

  TEST(Category, IdentityCommutesKleisliList){
    EXPECT_EQ(
      run(kleisli<ListMonad>)(compose(cat([](auto x){ return list( x, x); }), identity))('x'),
      run(kleisli<ListMonad>)(compose(identity, cat([](auto x){ return list( x, x); })))('x'));
  }

} // end of namespace Contextual::Details::Testing
