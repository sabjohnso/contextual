#pragma once

#include <rapidcheck.h>
#include <contextual/details/Run.hpp>
#include <contextual/details/Functor.hpp>

namespace Contextual::Laws
{
  using Details::run;

  constexpr auto fMap = Details::Functor::fMap;

  /**
   * @brief Functor identity: fMap(id, x) == x
   */
  template<typename Context, typename T>
  void functorIdentity(Context ctx, T x)
  {
    auto id = [](auto v) { return v; };
    RC_ASSERT(run(ctx)(fMap(id, x)) == x);
  }

  /**
   * @brief Functor composition: fMap(f . g, x) == fMap(f, fMap(g, x))
   */
  template<typename Context, typename F, typename G, typename T>
  void functorComposition(Context ctx, F f, G g, T x)
  {
    auto composed = [=](auto v) { return f(g(v)); };
    RC_ASSERT(
      run(ctx)(fMap(composed, x)) ==
      run(ctx)(fMap(f, run(ctx)(fMap(g, x)))));
  }

} // end of namespace Contextual::Laws
