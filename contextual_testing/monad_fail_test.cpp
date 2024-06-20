//
// ... Standard header files
//
#include <optional>
#include <functional>
#include <type_traits>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... External header files
//
#include <function_utility/function_utility.hpp>
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/MonadFail.hpp>
#include <contextual/details/Deriving.hpp>

using std::optional;
using std::nullopt_t;
using std::nullopt;
using std::result_of_t;

using FunctionUtility::curry;

using ListProcessing::Dynamic::list;
using ListProcessing::Dynamic::nil;
using ListProcessing::Dynamic::ListType;

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {

    constexpr auto fail = MonadFail::fail;
    constexpr auto letM = MonadFail::letM;
    constexpr auto returnM = MonadFail::returnM;

    class ProtoOptionalContext
    {
    public:
      static constexpr auto fail = []<typename T>(Type<T>){ return optional<T>{}; };
      static constexpr auto pure = []<typename T>(T x){ return optional<T>{x}; };
      static constexpr auto flatMap =
        []<typename F, typename T, typename R = result_of_t<F(T)>>
        (F f, optional<T> mx){return mx.has_value() ? f(mx.value()) : R{}; };
    };

    class OptionalContext
      : public Derive<ProtoOptionalContext, MixinMonadFail, MixinMonadFailUtility>
    {} constexpr optionalContext{};


    class ProtoListContext
    {
    public:
      static constexpr auto fail = []<typename T>(Type<T>){ return nil<T>; };
      static constexpr auto pure = []<typename T>(T x){ return list(x); };
      static constexpr auto flatMap = []<typename F, typename T>(F f, T xs){
        using R = decltype(f(head(xs)));
        const auto recur = [=](auto recur, T xs, R accum) -> R {
          return hasData(xs)
            ? recur(recur, tail(xs), rappend(f(head(xs)), accum))
            : reverse(accum);
        };
        return recur(recur, xs, R::nil);
      };

    };


    class ListContext
      : public Derive<ProtoListContext, MixinMonadFail, MixinMonadFailUtility>
    {} constexpr listContext{};

  } // end of anonymous namespace


  TEST(MonadFail, OptionalFail){
    EXPECT_FALSE(bool(optional<int>(run(optionalContext, fail(type<int>)))));
  }

  TEST(MonadFail, OptionalFailNoFail){
    EXPECT_EQ(
      run(optionalContext)(
        letM(optional(3), [=](auto x){ return
        letM(optional(4), [=](auto y){ return
              returnM(x+y); }); })),
      7);
  }

  TEST(MonadFail, OptionalFailFirstFail){
    EXPECT_EQ(
      run(optionalContext)(
        letM(fail(type<int>), [=](auto x){ return
        letM(optional(4),     [=](auto y){ return
              returnM(x+y); }); })),
      std::nullopt);
  }

  TEST(MonadFail, OptionalFailSecondFail){
    EXPECT_FALSE(
      bool(run(optionalContext)(
             letM(optional(3), [=](auto x){ return
             letM(fail(type<int>),        [=](auto y){ return
                   returnM(x+y); }); }))));
  }

  TEST(MonadFail, OptionalFailBothFail){
    EXPECT_FALSE(
      bool(run(optionalContext)(
             letM(fail(type<int>), [=](auto x){ return
             letM(fail(type<int>), [=](auto y){ return
                   returnM(x+y); }); }))));
  }

  TEST(MonadFail, ListFail){
    EXPECT_EQ(run(listContext, fail(type<int>)), nil<int>);
  }

  TEST(MonadFail, ListFailNoFail){
    EXPECT_EQ(
      run(listContext)(
        letM(list(1, 2), [=](auto x){ return
        letM(list(3, 4), [=](auto y){ return
              returnM(x + y); }); })),
      list(4, 5, 5, 6));
  }

  TEST(MonadFail, ListFailFirstFail){
    EXPECT_EQ(
      run(listContext)(
        letM(fail(type<int>),   [=](auto x){ return
        letM(list(3, 4), [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }

  TEST(MonadFail, ListFailSecondFail){
    EXPECT_EQ(
      run(listContext)(
        letM(list(1, 2),      [=](auto x){ return
        letM(fail(type<int>), [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }

  TEST(MonadFail, ListFailBothFail){
    EXPECT_EQ(
      run(listContext)(
        letM(fail(type<int>), [=](auto x){ return
        letM(fail(type<int>),  [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }
} // end of namespace Contextual::Details::Testing
