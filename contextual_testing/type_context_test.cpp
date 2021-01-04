//
// ... Standard header files
//
#include <string>

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
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>

//
// ... Contextual header files
//
#include <contextual/instances/TypeContext.hpp>
#include <contextual/Monad.hpp>
#include <contextual/Run.hpp>

using std::plus;
using TypeUtility::type;
using TypeUtility::Type;
using FunctionUtility::curry;

namespace Contextual::Instances::Testing
{

  namespace // anonymous
  {
    constexpr auto fma = [](auto x, auto y, auto z){ return x*y + z; };
  } // end of anonymous namespace

  TEST(TypeContext, fMap){
    using namespace ::Contextual::Instances::TypeContextNS;
      EXPECT_EQ(
        fMap([](auto x){ return x/2.0; }, type<int>),
        type<double>);
  }

  TEST(TypeContext, LetF){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      letF(type<int>, [](auto x){
        return x / 2.0;}),
      type<double>);
  }

  TEST(TypeContext, Pure){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(pure(3), type<int>);
  }

  TEST(TypeContext, ReturnM){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(returnM(3), type<int>);
  }

  TEST(TypeContext, FApply){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      fApply(fMap(curry<2>(plus{}), type<int>),
             type<long long>),
      type<long long>);
  }

  TEST(TypeContext, LiftM){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      liftM([](auto x){ return x+x; }, type<float>),
      type<float>);
  }

  TEST(TypeContext, LiftM2){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      liftM2(plus{}, type<int>, type<double>),
      type<double>);
  }

  TEST(TypeContext, LiftM3){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      liftM3(fma, type<double>, type<int>, type<long>),
      type<double>);
  }

  TEST(TypeContext, FlatMap){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(flatMap([](auto x){ return type<decltype(x)>; }, type<int>), type<int>);
  }

  TEST(TypeContext, LetM){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(
      letM(pure( 3   ), [ ](auto x){ return
      letM(pure( 4.0 ), [=](auto y){ return
            returnM(x * y); }); }),
      type<double>);
  }

  TEST(TypeContext, Flatten){
    using namespace ::Contextual::Instances::TypeContextNS;
    EXPECT_EQ(flatten(type<Type<int>>), type<int>);
  }

  TEST(TypeContext, GeneralPure){
    using namespace ::Contextual::MonadNS;
    EXPECT_EQ(run(typeContext, pure(3)), type<int>);
  }

  TEST(TypeContext, GeneralReturnM){
    using namespace ::Contextual::MonadNS;
    EXPECT_EQ(run(typeContext, returnM(3)), type<int>);
  }

  TEST(TypeContext, GeneralLetM){
    using namespace ::Contextual::MonadNS;
    EXPECT_EQ(run(typeContext)(
                letM(pure(3), [ ](auto x){ return
                letM(pure(4), [=](auto y){ return
                      returnM(x+y); }); })),
              type<int>);
  }

  TEST(TypeContext, GeneralFlatten){
    using namespace ::Contextual::MonadNS;
    EXPECT_EQ(run(typeContext, flatten(pure(pure(3)))), type<int>);
  }

} // end of namespace Contextual::Instances::Testing
