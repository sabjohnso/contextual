//
// ... Standard header files
//
#include <optional>
#include <type_traits>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Functor.hpp>
#include "laws/FunctorLaws.hpp"

namespace Contextual::Details::Testing
{
  using std::optional;
  using std::make_optional;
  using std::decay_t;
  using std::invoke_result_t;

  namespace // anonymous
  {
    constexpr
    class IdentityFunctor
    {
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx) { return std::forward<F>(f)(std::forward<T>(mx)); }
    } identityFunctor{};

    constexpr
    class OptionalFunctor
    {
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, optional<T> const& mx)
      {
        using Result = optional<decay_t<invoke_result_t<F, T>>>;
        return mx.has_value()
          ? Result(f(mx.value()))
          : Result();
      }
    } optionalFunctor{};

  } // end of anonymous namespace

  RC_GTEST_PROP(FunctorLaws, IdentityLaw, (int x))
  {
    Laws::functorIdentity(identityFunctor, x);
  }

  RC_GTEST_PROP(FunctorLaws, CompositionLaw, (int x))
  {
    auto f = [](int v) { return v * 2; };
    auto g = [](int v) { return v + 3; };
    Laws::functorComposition(identityFunctor, f, g, x);
  }

  RC_GTEST_PROP(FunctorLaws, OptionalIdentityLaw, (int x))
  {
    Laws::functorIdentity(optionalFunctor, make_optional(x));
  }

  RC_GTEST_PROP(FunctorLaws, OptionalCompositionLaw, (int x))
  {
    auto f = [](int v) { return v * 2; };
    auto g = [](int v) { return v + 3; };
    Laws::functorComposition(optionalFunctor, f, g, make_optional(x));
  }

  RC_GTEST_PROP(FunctorLaws, OptionalIdentityNone, ())
  {
    Laws::functorIdentity(optionalFunctor, optional<int>());
  }

} // end of namespace Contextual::Details::Testing
