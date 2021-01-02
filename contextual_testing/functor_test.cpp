//
// ... Standard header files
//
#include <utility>
#include <optional>
#include <type_traits>


//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Functor.hpp>

using std::pair;
using std::string;
using std::forward;
using std::optional;
using std::make_optional;
using std::decay_t;
using std::result_of_t;

namespace Contextual::Details::Testing
{

  namespace // anonymous
  {
    constexpr auto fMap = Functor::fMap;
    constexpr auto homogenize = Functor::homogenize;
    constexpr auto letF = Functor::letF;

    constexpr
    class IdentityFunctor{
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx){ return forward<F>(f)(forward<T>(mx)); }
    } identityFunctor{};

    constexpr
    class OptionalFunctor{
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, optional<T> const& mx){
        using Result = optional<decay_t<result_of_t<F(T)>>>;
        return mx.has_value()
          ? Result(f(mx.value()))
          : Result();
      }
    } optionalFunctor{};

    constexpr
    class ProductFunctor{
    public:
      template<typename F, typename T, typename U>
      static constexpr auto
      fMap(F&& f, pair<T,U> const& wx){
        return pair(f(wx.first), wx.second);
      }
    } productFunctor{};

    constexpr
    class FunctionFunctor{
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx){
        return [=](auto e){ return f(mx(e)); };
      }
    } functionFunctor{};


  } // end of anonymous namespace

  TEST(Functor, FMapIdentity){
    EXPECT_EQ(
      run(identityFunctor)(
        fMap([](auto x){ return x+x; }, 3)),
      6);
  }
  TEST(Functor, LetFIdentity){
    EXPECT_EQ(
      run(identityFunctor)(
        letF(3, [](auto x){
          return x*x;
        })),
      9);
  }

  TEST(Functor, HomogenizeIdentity){
    EXPECT_EQ(
      run(identityFunctor)(
        homogenize(3, 4)),
      3);
  }

  TEST(Functor, FMapOptionalPresent){
    EXPECT_EQ(
      run(optionalFunctor)(
        fMap([](auto x){ return x+x; }, make_optional(3))),
      make_optional(6));
  }

  TEST(Functor, LetFOptionalPresent){
    EXPECT_EQ(
      run(optionalFunctor)(
        letF(make_optional(3), [](auto x){
          return x+x;
        })),
      6);
  }

  TEST(Functor, LetFOptionalMissing){
    EXPECT_EQ(
        run(optionalFunctor)(
          letF(optional<int>(), [](auto x){
            return x+x;
          })),
        std::nullopt);
  }

  TEST(Functor, FMapOptionalMissing){
     EXPECT_FALSE(
       bool(run(optionalFunctor)(
              fMap([](auto x){ return x+x; }, optional<int>()))));
  }

  TEST(Functor,HomogenizeOptionalPresent){
    EXPECT_EQ(
      run(optionalFunctor)(
        homogenize(3, optional(4))),
      optional(3));
  }

  TEST(Functor,HomogenizeOptionalMissing){
    EXPECT_FALSE(
      bool(run(optionalFunctor)(
             homogenize(3, optional<int>()))));
  }


  TEST(Functor, FMapFunctionFunctor){
    EXPECT_EQ(
      run(5)(
        run(functionFunctor)(
          fMap([](auto x){ return x+x; },
               constant(3)))),
      6);
  }

  TEST(Functor, LetFunctionFunctor){
    EXPECT_EQ(
      run(5)(
        run(functionFunctor)(
          letF(constant(4), [](auto x){
            return x+x;
          }))),
      8);
  }

  TEST(Functor, HomogenizeFunctionFunctor){
    EXPECT_EQ(
      run(5)(
        run(functionFunctor)(
          homogenize(4, constant(3)))),
      4);
  }

  TEST(Functor, FMapProductFunctor){
    [xs = run(productFunctor)(
        fMap([](auto x){ return x+x; },
             pair(3, string("e"))))]{
      EXPECT_EQ(xs.first, 6);
      EXPECT_EQ(xs.second, string("e"));
    }();
  }

  TEST(Functor, LetFProductFunctor){
    [xs = run(productFunctor)(
        letF(pair(3, string("e")), [](auto x){
          return x+x;
        }))]{
      EXPECT_EQ(xs.first, 6);
      EXPECT_EQ(xs.second, string("e"));
    }();
  }




} // End of namespace Contextual::Details::Testing
