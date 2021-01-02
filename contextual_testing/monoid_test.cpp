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
#include <contextual/details/Monoid.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::plus;
    using std::multiplies;

    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::list;

    using ::Contextual::Details::run;
    constexpr auto mAppend = ::Contextual::Details::Monoid::mAppend;
    constexpr auto mEmpty = ::Contextual::Details::Monoid::mEmpty;


    class ProtoAddition {
    public:
      static constexpr int mEmpty = 0;
      static constexpr auto mAppend = curry<2>(plus{});
    };

    class Addition : public Derive<ProtoAddition, MixinMonoid>
    {} constexpr addition{};

    class ProtoMultiplication {
    public:
      static constexpr int mEmpty = 1;
      static constexpr auto mAppend = curry<2>(multiplies{});
    };

    class Multiplication  : public Derive<ProtoMultiplication, MixinMonoid>
    {} constexpr multiplication{};


    class ProtoAppend {
    public:
      static constexpr Nil mEmpty{};
      static constexpr auto mAppend = curry<2>([](auto xs, auto ys){ return append(xs, ys); });
    };

    class Append : public Derive<ProtoAppend, MixinMonoid>
    {} constexpr appendMonoid{};

  } // end of anonymous namespace

  TEST(Monoid, AdditionMEmpty){
    EXPECT_EQ(run(addition)(mEmpty), 0);
  }

  TEST(Monoid, AdditionMAppend){
    EXPECT_EQ(run(addition)(mAppend(3, 4)), 7);
  }

  TEST(Monoid, AdditionIdentity){
    EXPECT_EQ(run(addition)(mAppend(mEmpty, 3)),3);
  }

  TEST(Monoid, AdditionIdentityCommutes){
    EXPECT_EQ(
      run(addition)(mAppend(mEmpty, 3)),
      run(addition)(mAppend(3, mEmpty)));
  }

  TEST(Monoid, AdditionAssociative){
    EXPECT_EQ(
      run(addition)(mAppend(mAppend(3, 4), 5)),
      run(addition)(mAppend(3, mAppend(4, 5))));
  }

  TEST(Monoid, MultiplicationMEmpty){
    EXPECT_EQ(run(multiplication)(mEmpty), 1);
  }

  TEST(Monoid, MultiplicationMAppend){
    EXPECT_EQ(run(multiplication)(mAppend(3, 4)), 12);
  }

  TEST(Monoid, MultiplicationIdentity){
    EXPECT_EQ(run(multiplication)(mAppend(mEmpty, 3)),3);
  }

  TEST(Monoid, MultiplicationIdentityCommutes){
    EXPECT_EQ(
      run(multiplication)(mAppend(mEmpty, 3)),
      run(multiplication)(mAppend(3, mEmpty)));
  }

  TEST(Monoid, MultiplicationAssociative){
    EXPECT_EQ(
      run(multiplication)(mAppend(mAppend(3, 4), 5)),
      run(multiplication)(mAppend(3, mAppend(4, 5))));
  }


  TEST(Monoid, AppendMonoid){
    EXPECT_EQ(
      run(appendMonoid, mAppend(list(1,2), list(3, 4))),
      list(1, 2, 3, 4));
  }

  TEST(Monoid, AppendMonoidIdentity){
    EXPECT_EQ(
      run(appendMonoid)(mAppend(list(1,2), mEmpty)),
      list(1, 2));
  }

  TEST(Monoid, AppendMonoidIdentityCommutes){
    EXPECT_EQ(
      run(appendMonoid,mAppend(mEmpty, list(1, 2))),
      run(appendMonoid,mAppend(list(1, 2), mEmpty)));
  }

  TEST(Monoid, AppendMonoidAssociative){
    EXPECT_EQ(
      run(appendMonoid, mAppend(mAppend(list(1,2), list(3,4)), list(5, 6))),
      run(appendMonoid, mAppend(list(1,2), mAppend(list(3,4), list(5,6)))));
  }

  TEST(Monoid, VariadicAppend){

    EXPECT_EQ(
      run(appendMonoid, Monoid::mAppend_(list(1, 2), list(3, 4), list(5, 6))),
      list(1, 2, 3, 4, 5, 6));
  }

} // end of namespace Contextual::Details::Testing
