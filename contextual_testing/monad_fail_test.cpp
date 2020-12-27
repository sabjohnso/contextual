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

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    constexpr auto add = curry<2>(std::plus{});

    template<typename T>
    constexpr auto fail = MonadFail::fail<T>;
    constexpr auto letM = MonadFail::letM;
    constexpr auto returnM = MonadFail::returnM;

    constexpr
    class OptionalContext : public Deriving<
      OptionalContext,
      MonadFail::MonadicFMap,
      MonadFail::MonadicFApply,
      MonadFail::MonadicFlatten
      >
    {
    public:
      template<typename T>
      static constexpr auto
      fail(){ return  optional<T>{}; }

      template<typename T>
      static constexpr auto
      pure(T x){ return optional(x); }

      template<typename F>
      static constexpr auto
      flatMap(F, nullopt_t){
        return nullopt;
      }

      template<typename F, typename T, typename R = result_of_t<F(T)>>
      static constexpr auto
      flatMap(F f, optional<T> mx){
        return bool(mx)
          ? f(mx.value())
          : R();
      }
    } optionalContext{};


    constexpr
    class ListContext : public Deriving<
      ListContext,
      MonadFail::MonadicFMap,
      MonadFail::MonadicFApply,
      MonadFail::MonadicFlatten
      >{
    public:

      template<typename T>
      static constexpr auto
      fail(){ return nil<T>; }

      template<typename T>
      static constexpr auto
      pure(T x){ return list(x); }

      template<typename F, typename List, typename Result = result_of_t<F(typename List::value_type)>>
      static constexpr Result
      flatMap(F f, List mx){
        return hasData(mx)
          ? append( f(head(mx)), flatMap(f, tail(mx)))
          : Result::nil;
      }
    } listContext{};




  } // end of anonymous namespace


  TEST(MonadFail, OptionalFail){
    EXPECT_FALSE(bool(optional<int>(run(optionalContext, fail<int>))));
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
    EXPECT_FALSE(
      bool(run(optionalContext)(
             letM(fail<int>,   [=](auto x){ return
             letM(optional(4), [=](auto y){ return
                   returnM(x+y); }); }))));
  }

  TEST(MonadFail, OptionalFailSecondFail){
    EXPECT_FALSE(
      bool(run(optionalContext)(
             letM(optional(3), [=](auto x){ return
             letM(fail<int>,   [=](auto y){ return
                   returnM(x+y); }); }))));
  }

  TEST(MonadFail, OptionalFailBothFail){
    EXPECT_FALSE(
      bool(run(optionalContext)(
             letM(fail<int>,   [=](auto x){ return
             letM(fail<int>,   [=](auto y){ return
                   returnM(x+y); }); }))));
  }

  TEST(MonadFail, ListFail){
    EXPECT_EQ(run(listContext, fail<int>), nil<int>);
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
        letM(nil<int>,   [=](auto x){ return
        letM(list(3, 4), [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }

  TEST(MonadFail, ListFailSecondFail){
    EXPECT_EQ(
      run(listContext)(
        letM(list(1, 2), [=](auto x){ return
        letM(nil<int>,   [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }

  TEST(MonadFail, ListFailBothFail){
    EXPECT_EQ(
      run(listContext)(
        letM(list(1, 2), [=](auto x){ return
        letM(nil<int>,   [=](auto y){ return
              returnM(x + y); }); })),
      nil<int>);
  }
} // end of namespace Contextual::Details::Testing
