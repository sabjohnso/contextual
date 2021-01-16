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
#include <function_utility/function_utility.hpp>

//
// ... Contextual header files
//
#include <contextual/Run.hpp>
#include <contextual/instances/FunctionContext.hpp>

namespace Contextual::Instances::Testing
{
  namespace // anonymous
  {
    using std::plus;
    using FunctionUtility::curry;

    constexpr auto add = curry<2>(plus{});
    constexpr auto twc = curry<1>([](auto x){ return x+x; });

  } // end of anonymous namespace

  TEST(FunctionContext, FMap){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run('e', fMap(twc, pure(3))), 6);
  }

  TEST(FunctionContext, LetF){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run('e')(
                letF(pure(3), [](auto x){
                  return x+x; })),
              6);
  }

  TEST(FunctionContext, Pure){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(pure('x', 'e'), 'x');
    EXPECT_EQ(pure('x')('e'), 'x');
    EXPECT_EQ(run('e', pure('x')), 'x');
  }

  TEST(FunctionContext, ReturnM){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(returnM('x', 'e'), 'x');
    EXPECT_EQ(returnM('x')('e'), 'x');
    EXPECT_EQ(run('e', returnM('x')), 'x');
  }

  TEST(FunctionContext, FApply){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run(4, fApply(fMap(add, pure(3)), ask)), 7);
  }

  TEST(FunctionContext, LiftM){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run(3, liftM(twc, ask)), 6);
  }

  TEST(FunctionContext, LiftM2){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run(3, liftM2(add, pure(4), ask)), 7);
  }

  TEST(FunctionContext, FlatMap){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(run(3, flatMap(pure, ask)), 3);
  }

  TEST(FunctionContext, LetM){
    using namespace Contextual::Instances::FunctionContextNS;
    EXPECT_EQ(
      run(3)(
        letM(ask,     [ ](auto x){ return
        letM(pure(4), [=](auto y){ return
              returnM(x+y); }); })),
      7);
  }






} // end of namespace Contextual::Instances::Testing
