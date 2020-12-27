//
// ... Standard header files
//
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
#include <contextual/details/MonadPlus.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::result_of_t;

    using ListProcessing::Dynamic::nil;
    using ListProcessing::Dynamic::list;

    template<typename T>
    constexpr auto mZero = MonadPlus::mZero<T>;
    constexpr auto mPlus = MonadPlus::mPlus;

    constexpr
    class ListContext : Deriving<
      ListContext,
      MonadPlus::MonadicFMap,
      MonadPlus::MonadicFApply,
      MonadPlus::MonadicFlatten
      >{
    public:

      template<typename T>
      static auto
      fail(){ return nil<T>; }

      template<typename List1, typename List2>
      static auto
      mPlus(List1 xs, List2 ys){ return append(xs, ys); }

      template<typename T>
      static auto
      pure(T x){ return list(x); }

      template<typename F, typename List, typename Result = result_of_t<F(typename List::value_type)>>
      static Result
      flatMap(F f, List xs){
        return hasData(xs)
          ? mplus(f(head(xs)), flatMap(f, tail(xs)))
          : Result::nil;
      }


    } listContext{};

  } // end of anonymous namespace

  TEST(MonadPlus, ListMPlus){
    EXPECT_EQ(
      run(listContext)(
        mPlus(list(1,2), list(3,4))),
      list(1, 2, 3, 4));
  }

  TEST(MonadPlus, ListMPlusMZero){
    EXPECT_EQ(
      run(listContext)(
        mPlus(list(1, 2), mZero<int>)),
      list(1, 2));
  }

  TEST(MonadPlus, ListMPlusMzeroCommutes){
    EXPECT_EQ(
      run(listContext, mPlus(list(1, 2), mZero<int>)),
      run(listContext, mPlus(mZero<int>, list(1, 2))));
  }
} // end of namespace Contextual::Details::Testing
