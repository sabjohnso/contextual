//
// ... Standard header files
//
#include <utility>
#include <functional>
#include <optional>
#include <type_traits>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Functor.hpp>
#include <contextual/details/Applicative.hpp>

using std::forward;
using std::result_of_t;
using std::decay_t;
using std::function;
using std::optional;
using std::make_optional;
using std::nullopt;

namespace Contextual::Details::Testing
{
  namespace
  {
    constexpr auto fMap   = Applicative::fMap;
    constexpr auto pure   = Applicative::pure;
    constexpr auto fApply = Applicative::fApply;


    constexpr
    class IdentityApplicative
      : public Deriving<IdentityApplicative, Applicative::ApplicativeFMap> {
    public:
      template<typename T>
      static constexpr auto
      pure(T&& x){ return x; }

      template<typename F, typename T>
      static constexpr auto
      fApply(F&& f, T&& x){
        return std::forward<F>(f)(std::forward<T>(x));
      }
    } identityApplicative{};

    constexpr
    class OptionalApplicative
      : public Deriving<OptionalApplicative, Applicative::ApplicativeFMap> {
    public:
      template<typename T>
      static constexpr auto
      pure(T&& x){ return make_optional(std::forward<T>(x)); }

      template<typename F, typename T>
      static constexpr auto
      fApply(optional<F> const& mf, optional<T> const& mx){
        using Result = optional<decay_t<result_of_t<F(T)>>>;
        return mf.has_value() && mx.has_value()
          ? Result(mf.value()(mx.value()))
          : Result();
      }
    } optionalApplicative{};

  } // end of anonymous namespace


  TEST(Applicative, PureIdentity){
    EXPECT_EQ(
      run(identityApplicative)(
        pure(3)),
      3);
  }

  TEST(Applicative, FApplyIdentity){
    EXPECT_EQ(
      run(identityApplicative)(
        fApply(pure([](auto x){ return x+x; }),
               pure(3))),
      6);
  }

  TEST(Applicative, FMapIdentity){
    EXPECT_EQ(
      run(identityApplicative)(
        fMap([](auto x){ return x*x; }, pure(3))),
      9);
  }

  TEST(Applicative, PureOptional){
    EXPECT_EQ(
      run(optionalApplicative)(
        pure(3)),
      optional(3));
  }

  TEST(Applicative, FApplyOptionalPresent){
    EXPECT_EQ(
      run(optionalApplicative)(
        fApply(pure([](auto x){ return x+x; }),
               pure(3))),
      6);
  }

  TEST(Applicative, FApplyOptionalMissingFunction){
    EXPECT_EQ(
      run(optionalApplicative)(
        fApply(optional<function<int(int)>>(),
               pure(3))),
      nullopt);
  }

  TEST(Applicative, FApplyOptionalMissingValue){
    EXPECT_EQ(
      run(optionalApplicative)(
        fApply(pure([](auto x){ return x*x; }),
               optional<int>())),
      nullopt);
  }

  TEST(Applicative, FApplyOptionalMissingBoth){
    EXPECT_EQ(
      run(optionalApplicative)(
        fApply(optional<function<int(int)>>(),
               optional<int>())),
      nullopt);
  }



} // end of namespace Contextual::Details
