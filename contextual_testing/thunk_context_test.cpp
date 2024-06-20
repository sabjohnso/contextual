//
// ... Standard header files
//
#include <string>
#include <sstream>
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
// ... Context header files
//
#include <contextual/instances/ThunkContext.hpp>

using std::string;
using std::plus;
using FunctionUtility::curry;

namespace Contextual::Instances::Testing
{
  namespace // anonymous
  {
    constexpr auto add = curry<2>(plus{});
    constexpr auto dup = curry<2>([](auto f, auto x){ return f(x, x); });
    constexpr auto twc = dup(add);


  } // end of anonymous namespacee

  TEST(ThunkContext, FMap){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(
      force(fMap(twc, []{return string("x"); })),
      string("xx"));

  }

  TEST(ThunkContext, LetF){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(
      force(
        letF(pure(string("x")), [](auto x){
          return x+x; })),
      string("xx"));
  }

  TEST(ThunkContext, Pure){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(force(pure('x')), 'x');
  }

  TEST(ThunkContext, FApply){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(force(fApply(fMap(add, pure(string("x"))), pure(string("y")))),
              string("xy"));
  }

  TEST(ThunkContext, LiftM){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(force(liftM(twc, pure(string("x")))),
              string("xx"));
  }

  TEST(ThunkContext, LiftM2){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(force(liftM2(add, pure(string("x")), pure(string("y")))),
              string("xy"));
  }

  TEST(ThunkContext, FlatMap){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(
      force(flatMap([](auto x){ return pure(twc(x)); }, pure(string("x")))),
      string("xx"));
  }

  TEST(ThunkContext, LetM){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(
      force(letM(pure(string("x")), [ ](auto x){ return
            letM(pure(string("y")), [=](auto y){ return
            letM(pure(string("z")), [=](auto z){ return
                  returnM(x + y + z); }); }); })),
      string("xyz"));
  }

  TEST(ThunkContext, Flatten){
    using namespace Contextual::Instances::ThunkContextNS;
    EXPECT_EQ(
      force(flatten(pure(pure(string("x"))))),
      string("x"));
  }



} // end of namespace Contextual::Instances::Testing
