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
#include <contextual/details/Monoid.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::list;

    using ::Contextual::Details::run;
    constexpr auto mappend = ::Contextual::Details::Monoid::mappend;
    constexpr auto mempty = ::Contextual::Details::Monoid::mempty;

    constexpr
    class Addition {
    public:
      static constexpr int mempty = 0;
      template<typename T, typename U>
      static constexpr auto
      op(T&& x, U&& y){ return x + y; }
    } addition{};

    constexpr
    class Multiplication {
    public:
      static constexpr int mempty = 1;
      template<typename T, typename U>
      static constexpr auto
      op(T&& x, U&& y){ return x * y; }
    } multiplication{};

    constexpr
    class Append {
    public:
      static constexpr Nil mempty{};

      template<typename T, typename U>
      static constexpr auto
      op(T&& xs, U&& ys){ return append(xs, ys); }
    } appendMonoid{};

  } // end of anonymous namespace

  TEST(Monoid, AdditionMempty){
    EXPECT_EQ(run(addition)(mempty), 0);
  }

  TEST(Monoid, AdditionMappend){
    EXPECT_EQ(run(addition)(mappend(3, 4)), 7);
  }

  TEST(Monoid, AdditionIdentity){
    EXPECT_EQ(run(addition)(mappend(mempty, 3)),3);
  }

  TEST(Monoid, AdditionIdentityCommutes){
    EXPECT_EQ(
      run(addition)(mappend(mempty, 3)),
      run(addition)(mappend(3, mempty)));
  }

  TEST(Monoid, AdditionAssociative){
    EXPECT_EQ(
      run(addition)(mappend(mappend(3, 4), 5)),
      run(addition)(mappend(3, mappend(4, 5))));
  }

  TEST(Monoid, MultiplicationMempty){
    EXPECT_EQ(run(multiplication)(mempty), 1);
  }

  TEST(Monoid, MultiplicationMappend){
    EXPECT_EQ(run(multiplication)(mappend(3, 4)), 12);
  }

  TEST(Monoid, MultiplicationIdentity){
    EXPECT_EQ(run(multiplication)(mappend(mempty, 3)),3);
  }

  TEST(Monoid, MultiplicationIdentityCommutes){
    EXPECT_EQ(
      run(multiplication)(mappend(mempty, 3)),
      run(multiplication)(mappend(3, mempty)));
  }

  TEST(Monoid, MultiplicationAssociative){
    EXPECT_EQ(
      run(multiplication)(mappend(mappend(3, 4), 5)),
      run(multiplication)(mappend(3, mappend(4, 5))));
  }


  TEST(Monoid, AppendMonoid){
    EXPECT_EQ(
      run(appendMonoid, mappend(list(1,2), list(3, 4))),
      list(1, 2, 3, 4));
  }

  TEST(Monoid, AppendMonoidIdentity){
    EXPECT_EQ(
      run(appendMonoid)(mappend(list(1,2), mempty)),
      list(1, 2));
  }

  TEST(Monoid, AppendMonoidIdentityCommutes){
    EXPECT_EQ(
      run(appendMonoid,mappend(mempty, list(1, 2))),
      run(appendMonoid,mappend(list(1, 2), mempty)));
  }

  TEST(Monoid, AppendMonoidAssociative){
    EXPECT_EQ(
      run(appendMonoid, mappend(mappend(list(1,2), list(3,4)), list(5, 6))),
      run(appendMonoid, mappend(list(1,2), mappend(list(3,4), list(5,6)))));
  }

  TEST(Monoid, VariadicAppend){
    EXPECT_EQ(
      run(appendMonoid, Monoid::mappend_(list(1, 2), list(3, 4), list(5, 6))),
      list(1, 2, 3, 4, 5, 6));
  }

} // end of namespace Contextual::Details::Testing
