//
// ... Standard header files
//
#include <functional>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <contextual_testing/static_checks.hpp>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>
#include <list_processing/compile_time.hpp>

//
// ... Contextual header files
//
#include <contextual/Run.hpp>
#include <contextual/MonadPlus.hpp>
#include <contextual/instances/CTListContext.hpp>

using std::plus;
using TypeUtility::type;
using FunctionUtility::curry;
using ListProcessing::CompileTime::list;
using ListProcessing::CompileTime::nothing;

namespace Contextual::Instances::Testing
{
  namespace // anonymous
  {

    constexpr auto sqr = curry<1>([](auto x){ return x*x; });
    constexpr auto add = curry<2>(plus{});
    constexpr auto fma = curry<3>([](auto x, auto y, auto z){ return x*y + z; });
    constexpr auto let = curry<2>([](auto x, auto f){ return f(x); });

  } // end of anonymous namespace

  TEST(CTListContext, FMap){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      fMap([](auto x){ return x*x; }, list(1, 2, 3)),
      list(1, 4, 9));
  }

  TEST(CTListContext, LetF){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      letF(list(1, 2, 3), [](auto x){
        return x*x; }),
      list(1, 4, 9));
  }

  TEST(CTListContext, Pure){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      pure(1),
      list(1));
  }

  TEST(CTListContext, FApply){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      fApply(fMap(add, list(1, 2)), list(3, 4)),
      list(4, 5, 5, 6));
  }

  TEST(CTListContext, FApply2){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      fApply2(pure(add), list(1, 2), list(3, 4)),
      list(4, 5, 5, 6));
  }

  TEST(CTListContext, FApply3){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      fApply3(pure(fma), list(1, 2), list(3, 4), list(5, 6)),
      list(8, 9, 9, 10, 11, 12, 13, 14));
  }

  TEST(CTListContext, LiftM){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      liftM(sqr, list(1, 2, 3)),
      list(1, 4, 9));
  }

  TEST(CTListContext, LiftM2){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      liftM2(add, list(1, 2), list(3, 4)),
      list(4, 5, 5, 6));
  }

  TEST(CTListContext, LiftM3){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      liftM3(fma, list(1, 2), list(3, 4), list(5, 6)),
      list(8, 9, 9, 10, 11, 12, 13, 14));
  }

  TEST(CTListContext, FlatMap){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      let(fMap(add, list(1, 2)), [](auto fs){ return
      let([=](auto x){ return fMap([=](auto f){ return f(x); }, fs); }, [=](auto f){
        return flatMap(f, list(3, 4)); }); }),
      list(4, 5, 5, 6));
  }

  TEST(CTListContext, LetM){
    using namespace Contextual::Instances::CTListContextNS;
    // clang-format off
    STATIC_EXPECT_EQ(
      letM(list(1, 2), [ ](auto x){ return
      letM(list(3, 4), [=](auto y){ return
      letM(list(5, 6), [=](auto z){ return
            returnM(x*y + z); }); }); }),
      list(8, 9, 9, 10, 11, 12, 13, 14));
    // clang-format on
  }

  TEST(CTListContext, Flatten1){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      flatten(pure(pure(1))),
      list(1));
  }

  TEST(CTListContext, GenericFlatten1){
    using Contextual::run;
    constexpr auto pure = Contextual::MonadPlus::pure;
    constexpr auto flatten = Contextual::MonadPlus::flatten;
    STATIC_EXPECT_EQ(
      run(CTListContext{})(
        flatten(pure(pure(1)))),
      list(1));
  }

  TEST(CTListContext, Flatten){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      flatten(list(list(1, 2), list(3, 4))),
      list(1, 2, 3, 4));
  }

  TEST(CTListContext, GenericFlatten){
    using Contextual::run;
    constexpr auto flatten = Contextual::MonadPlus::flatten;
    STATIC_EXPECT_EQ(
      run(CTListContext{})(
        flatten(list(list(1, 2), list(3, 4)))),
      list(1, 2, 3, 4));
  }

  TEST(CTListContext, MZero){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mZero(type<int>),
      nothing);
  }

  TEST(CTListContext, MPlus){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mPlus(list(1, 2), list(3, 4)),
      list(1, 2, 3, 4));
  }

  TEST(CTListContext, MPlusMZero){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mPlus(list(1, 2), mZero(type<int>)),
      list(1, 2));
  }

  TEST(CTListContext, MZeroMPlus){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mPlus(mZero(type<int>), list(1, 2)),
      list(1, 2));
  }

  TEST(CTListContext, MEmpty){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(mEmpty, nothing);
  }

  TEST(CTListContext, MAppend){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mAppend(list(1, 2), list(3, 4)),
      list(1, 2, 3, 4));
  }

  TEST(CTListContext, MAppendMEmpty){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mAppend(list(1, 2), mEmpty),
      list(1, 2));
  }

  TEST(CTListContext, MEmptyMAppend){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mAppend(mEmpty, list(1, 2)),
      list(1, 2));
  }

  TEST(CTListContext, MPlusMempty){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mPlus(list(1, 2), mEmpty),
      list(1,2));
  }

  TEST(CTListContext, MemptyMPlus){
    using namespace Contextual::Instances::CTListContextNS;
    STATIC_EXPECT_EQ(
      mPlus(mEmpty, list(1, 2)),
      list(1, 2));
  }




} // end of namespace Contextual::Instances::Testing
