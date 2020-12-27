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
#include <contextual/details/import.hpp>
#include <contextual/details/Run.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Semigroup.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using ListProcessing::Dynamic::list;
    constexpr auto op = Semigroup::op;
    constexpr auto op_ = Semigroup::op_;

    class Addition {
    public:
      template<typename T, typename U>
      static constexpr auto
      op(T&& x, U&& y){ return forward<T>(x) + forward<U>(y); }
    } addition{};

    constexpr
    class Multiplication {
    public:
      template<typename T, typename U>
      static constexpr auto
      op(T&& x, U&& y){ return forward<T>(x) * forward<U>(y); }
    } multiplication{};

    constexpr
    class Append {
    public:
      template<typename T, typename U>
      static constexpr auto
      op(T&& x, U&& y){ return append(forward<T>(x), forward<U>(y)); }
    } appendSemigroup{};

  } // end of anonymous namespace

  TEST(Semigroup, Addition){
    EXPECT_EQ(run(addition)(op(3, 4)), 7);
  }

  TEST(Semigroup, VariadicAddition){
    EXPECT_EQ(run(addition)(op_(1, 2, 3, 4)), 10);
  }


  TEST(Magma, Multiplication){
    EXPECT_EQ(run(multiplication, op(3, 4)), 12);
  }

  TEST(Magma, VariadicMultiplication){
    EXPECT_EQ(run(multiplication, op_(1, 2, 3, 4)), 24);
  }

  TEST(Magma, Append){
    EXPECT_EQ(
      run(appendSemigroup)(
        op(list(1,2), list(3,4))),
      list(1, 2, 3, 4));
  }

  TEST(Magma, VariadicAppend){
    EXPECT_EQ(
      run(appendSemigroup)(
        op_(list(1,2), list(3,4), list(5, 6))),
      list(1, 2, 3, 4, 5, 6));
  }



} // end of namespace Contextual::Details::Testing
