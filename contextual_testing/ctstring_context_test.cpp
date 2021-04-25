//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <contextual_testing/static_checks.hpp>

//
// ... External header files
//
#include <ctstring/macros.hpp>

//
// ... Contextual header files
//
#include <contextual/Run.hpp>
#include <contextual/Monoid.hpp>
#include <contextual/instances/CTStringContext.hpp>

namespace Contextual::Instances::Testing
{

  TEST(CTStringContext, MEmpty){
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(mEmpty, HOIST_STRING(""));
  }

  TEST(CTStringContext, GenericMEmpty){
    constexpr auto mEmpty = Contextual::Monoid::mEmpty;
    STATIC_EXPECT_EQ(
      run(CTStringContext(), mEmpty),
      HOIST_STRING(""));
  }

  TEST(CTStringContext, MAppend){
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(
      mAppend(HOIST_STRING("abc"), HOIST_STRING("123")),
      HOIST_STRING("abc123"));
  }

  TEST(CTStringContext, GenericMAppend){
    constexpr auto mAppend = Contextual::Monoid::mAppend;
    STATIC_EXPECT_EQ(
      run(CTStringContext())(
        mAppend(HOIST_STRING("abc"), HOIST_STRING("123"))),
      HOIST_STRING("abc123"));
  }

  TEST(CTStringContext, MAppendMEmpty){
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(
      mAppend(HOIST_STRING("abc"), mEmpty),
      HOIST_STRING("abc"));
  }

  TEST(CTStringContext, GenericMAppendMEmpty){
    constexpr auto mAppend = Contextual::Monoid::mAppend;
    constexpr auto mEmpty = Contextual::Monoid::mEmpty;
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(
      run(CTStringContext{}, mAppend(HOIST_STRING("abc"), mEmpty)),
      HOIST_STRING("abc"));
  }

  TEST(CTStringContext, MEmptyMAppend){
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(
      mAppend(mEmpty, HOIST_STRING("abc")),
      HOIST_STRING("abc"));
  }

  TEST(CTStringContext, GenericMEmptyMAppend){
    constexpr auto mAppend = Contextual::Monoid::mAppend;
    constexpr auto mEmpty = Contextual::Monoid::mEmpty;
    using namespace Contextual::Instances::CTStringContextNS;
    STATIC_EXPECT_EQ(
      run(CTStringContext{}, mAppend(mEmpty, HOIST_STRING("abc"))),
      HOIST_STRING("abc"));
  }
} // end of namespace Contextual::Instances::Testing
