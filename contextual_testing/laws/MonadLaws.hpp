#pragma once

#include <rapidcheck.h>
#include <contextual/details/Run.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Laws
{
  using Details::run;

  /**
   * @brief Monad left identity: flatMap(f, pure(x)) == f(x)
   *
   * Applying a function to a pure value via flatMap is the same
   * as applying the function directly.
   */
  template<typename Context, typename F, typename T>
  void monadLeftIdentity(Context ctx, F f, T x)
  {
    RC_ASSERT(ctx.flatMap(f, ctx.pure(x)) == f(x));
  }

  /**
   * @brief Monad right identity: flatMap(pure, mx) == mx
   *
   * Binding pure to a monadic value returns the same value.
   */
  template<typename Context, typename T>
  void monadRightIdentity(Context ctx, T mx)
  {
    RC_ASSERT(ctx.flatMap(ctx.pure, mx) == mx);
  }

  /**
   * @brief Monad associativity: flatMap(g, flatMap(f, mx)) == flatMap(h, mx)
   *   where h(x) = flatMap(g, f(x))
   *
   * Binding is associative: the order of nesting does not matter.
   */
  template<typename Context, typename F, typename G, typename T>
  void monadAssociativity(Context ctx, F f, G g, T mx)
  {
    auto h = [&](auto x) { return ctx.flatMap(g, f(x)); };
    RC_ASSERT(
      ctx.flatMap(g, ctx.flatMap(f, mx)) ==
      ctx.flatMap(h, mx));
  }

} // end of namespace Contextual::Laws
