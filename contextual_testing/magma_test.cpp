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
#include <contextual/details/Magma.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using ListProcessing::Dynamic::list;
    constexpr auto op = Magma::op;

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
    } appendMagma{};

  } // end of anonymous namespace

  TEST(Magma, Addition){
    EXPECT_EQ(run(addition)(op(3, 4)), 7);
  }

  TEST(Magma, Multiplication){
    EXPECT_EQ(run(multiplication, op(3, 4)), 12);
  }

  TEST(Magma, Append){
    EXPECT_EQ(
      run(appendMagma)(
        op(list(1,2), list(3,4))),
      list(1, 2, 3, 4));
  }


} // end of namespace Contextual::Details::Testing
