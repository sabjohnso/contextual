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
#include <contextual/details/Semigroup.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::plus;
    using std::multiplies;

    using ListProcessing::Dynamic::list;
    constexpr auto op = Semigroup::op;
    constexpr auto op_ = Semigroup::op_;

    class ProtoAddition {
    public:
      static constexpr auto op = curry<2>(plus{});
    };

    class Addition : public Derive<ProtoAddition, MixinSemigroup>
    {} constexpr addition{};



    class ProtoMultiplication {
    public:
      static constexpr auto op = curry<2>(multiplies{});
    };

    class Multiplication : public Derive<ProtoMultiplication, MixinSemigroup>
    {} constexpr multiplication{};



    class ProtoAppend {
      class Op_ : public Static_callable<Op_>{
      public:
        template<typename T, typename U, typename ... Vs>
        static constexpr auto
        call(T&& x, U&& y, Vs&& ... zs){
          if constexpr (count_types<Vs...>() == 0){
            return append(std::forward<T>(x), std::forward<U>(y));
          } else {
            return call(append(std::forward<T>(x), std::forward<T>(y)), std::forward<Vs>(zs) ...);
          }
        }
      };
    public:
      static constexpr Op_ op_{};
    };

    class Append : public Derive<ProtoAppend, MixinSemigroup>
    {} constexpr appendSemigroup{};

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
