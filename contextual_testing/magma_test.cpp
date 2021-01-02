//
// ... Standard header files
//
#include <functional>

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
    using std::plus;
    using std::multiplies;

    using ListProcessing::Dynamic::list;
    constexpr auto op = Magma::op;

    class Addition {
    public:
      static constexpr auto op = curry<2>(plus{});
    } addition{};
    static_assert(HasMagma<Addition>);


    constexpr
    class Multiplication {
    public:
      static constexpr auto op = curry<2>(multiplies{});
    } multiplication{};
    static_assert(HasMagma<Multiplication>);

    constexpr
    class Append {
    public:
      static constexpr auto op = curry<2>([](auto x, auto y){ return append(x, y); });
    } appendMagma{};
    static_assert(HasMagma<Append>);

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
