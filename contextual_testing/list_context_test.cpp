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
#include <contextual/Run.hpp>
#include <contextual/Monoid.hpp>
#include <contextual/MonadPlus.hpp>
#include <contextual/instances/ListContext.hpp>

using std::plus;
using TypeUtility::type;
using FunctionUtility::curry;
using ListProcessing::Dynamic::Nil;
using ListProcessing::Dynamic::nil;
using ListProcessing::Dynamic::list;
using ListProcessing::Dynamic::ListType;

namespace Contextual::Instances::Testing
{
  namespace // anonymous
  {
    template<typename T>
    std::ostream&
    operator <<(std::ostream& os, ListType<T> xs){
      os << "(";
      if(hasData(xs)){
        os << head(xs);
        doList(tail(xs), [&](auto x){
          os << " " << x;
        });
      }
      os << ")";
      return os;
    }
  } // end of anonymous namespace

  TEST(ListContext, FMap){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      fMap([](auto x){ return x+1; }, list(1, 2)),
      list(2,3));
  }

  TEST(ListContext, LetF){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      letF(list(1, 2), [](auto x){ return
            x+1; }),
      list(2, 3));
  }

  TEST(ListContext, Pure){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(pure(1), list(1));
  }

  TEST(ListContext, ReturnM){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(returnM(1), list(1));
  }

  TEST(ListContext, FApply){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      fApply(fMap(curry<2>(plus{}), list(1, 2)), list(3, 4)),
      list(4, 5, 5, 6));
  }

  TEST(ListContext, LiftM){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      liftM([](auto x){ return x+1; }, list(1, 2)),
      list(2, 3));
  }

  TEST(ListContext, LiftM2){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      liftM2(plus{}, list(1, 2), list(3, 4)),
      list(4, 5, 5, 6));
  }

  TEST(ListContext, LetM){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      flatMap([ ](auto x){ return list(x+1); }, list(1, 2)),
      list(2, 3));
  }

  TEST(ListContext, Flatten){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      flatten(list(list(1, 2), list(3, 4, 5))),
      list(1, 2, 3, 4, 5));
  }

  TEST(ListContext, MZero){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      mZero(type<int>),
      nil<int>);
  }

  TEST(ListContext, MPlus){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(
      mPlus(list(1, 2), list(3, 4)),
      list(1, 2, 3, 4));
  }

  TEST(ListContext, MPlusMZero){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mPlus(list(1, 2), mZero(type<int>)),
               list(1, 2));
  }

  TEST(ListContext, MPlusMZeroCommutes){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mPlus(list(1, 2), mZero(type<int>)),
               mPlus(mZero(type<int>), list(1, 2)));
  }

  TEST(ListContext, MEmpty){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mEmpty, Nil{});
  }

  TEST(ListContext, MAppend){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mAppend(list(1, 2), list(3, 4)),
               list(1, 2, 3, 4));
  }

  TEST(ListContext, MAppendAssociative){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mAppend(mAppend(list(1, 2), list(3, 4)), list(5, 6)),
               mAppend(list(1, 2), mAppend(list(3, 4), list(5, 6))));
  }

  TEST(ListContext, MAppendVariadic){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mAppend(mAppend(list(1, 2), list(3, 4)), list(5, 6)),
               mAppend_(list(1, 2),list(3, 4), list(5, 6)));
  }

  TEST(ListContext, MAppendMEmpty){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ( mAppend(list(1, 2), mEmpty),
               list(1, 2));
  }

  TEST(ListContext, MAppendMEmptyCommutes){
    using namespace ::Contextual::Instances::ListContextNS;
    EXPECT_EQ(mAppend(list(1, 2), mEmpty),
              mAppend(mEmpty, list(1, 2)));
  }


} // end of namespace Contextual::Instances::Testing
