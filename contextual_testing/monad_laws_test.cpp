//
// ... Standard header files
//
#include <type_traits>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

//
// ... External header files
//
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Monad.hpp>
#include "laws/MonadLaws.hpp"

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::invoke_result_t;

    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::ListType;
    using ListProcessing::Dynamic::list;

    class ProtoIdentityMonad
    {
    public:
      static constexpr auto fMap    = [](auto f, auto mx) { return f(mx); };
      static constexpr auto pure    = [](auto x) { return x; };
      static constexpr auto flatten = [](auto mmx) { return mmx; };
    };

    class IdentityMonad : public Derive<ProtoIdentityMonad, MixinMonad>
    {} constexpr identityMonad{};

    class ProtoListMonad
    {
      class FlatMap : public Static_curried<FlatMap, Nat<2>>
      {
      public:
        template<typename F>
        static Nil
        call(F, Nil) { return Nil{}; }

        template<typename F, typename T,
                 typename R = invoke_result_t<F, typename T::value_type>>
        static R
        call(F f, T xs)
        {
          return hasData(xs)
            ? append(f(head(xs)), call(f, tail(xs)))
            : R::nil;
        }
      };

    public:
      static constexpr auto pure = [](auto x) { return list(x); };
      static constexpr FlatMap flatMap{};
    };

    class ListMonad : public Derive<ProtoListMonad, MixinMonad>
    {} constexpr listMonad{};

  } // end of anonymous namespace

  // --- Identity monad laws ---

  RC_GTEST_PROP(MonadLaws, IdentityLeftIdentity, (int x))
  {
    auto f = [](auto v) { return v * 2; };
    Laws::monadLeftIdentity(identityMonad, f, x);
  }

  RC_GTEST_PROP(MonadLaws, IdentityRightIdentity, (int x))
  {
    Laws::monadRightIdentity(identityMonad, x);
  }

  RC_GTEST_PROP(MonadLaws, IdentityAssociativity, (int x))
  {
    auto f = [](auto v) { return v + 1; };
    auto g = [](auto v) { return v * 2; };
    Laws::monadAssociativity(identityMonad, f, g, x);
  }

  // --- List monad laws ---

  RC_GTEST_PROP(MonadLaws, ListLeftIdentity, (int x))
  {
    auto f = [](auto v) { return list(v, v); };
    Laws::monadLeftIdentity(listMonad, f, x);
  }

  RC_GTEST_PROP(MonadLaws, ListRightIdentity, (int x))
  {
    auto mx = list(x);
    Laws::monadRightIdentity(listMonad, mx);
  }

  RC_GTEST_PROP(MonadLaws, ListAssociativity, (int x))
  {
    auto f = [](auto v) { return list(v, v + 1); };
    auto g = [](auto v) { return list(v * 2); };
    auto mx = list(x);
    Laws::monadAssociativity(listMonad, f, g, mx);
  }

} // end of namespace Contextual::Details::Testing
