#pragma once

#include <rapidcheck.h>
#include <contextual/details/Run.hpp>
#include <contextual/details/Category.hpp>

namespace Contextual::Laws
{
  using Details::run;

  constexpr auto cat      = Details::Category::cat;
  constexpr auto compose  = Details::Category::compose;
  constexpr auto identity = Details::Category::identity;

  /**
   * @brief Category left identity: compose(f, identity) == f
   */
  template<typename Context, typename F, typename T>
  void categoryLeftIdentity(Context ctx, F f, T x)
  {
    RC_ASSERT(
      run(x)(run(ctx)(compose(cat(f), identity))) ==
      run(x)(run(ctx)(cat(f))));
  }

  /**
   * @brief Category right identity: compose(identity, f) == f
   */
  template<typename Context, typename F, typename T>
  void categoryRightIdentity(Context ctx, F f, T x)
  {
    RC_ASSERT(
      run(x)(run(ctx)(compose(identity, cat(f)))) ==
      run(x)(run(ctx)(cat(f))));
  }

  /**
   * @brief Category associativity: compose(f, compose(g, h)) == compose(compose(f, g), h)
   */
  template<typename Context, typename F, typename G, typename H, typename T>
  void categoryAssociativity(Context ctx, F f, G g, H h, T x)
  {
    RC_ASSERT(
      run(x)(run(ctx)(compose(cat(f), compose(cat(g), cat(h))))) ==
      run(x)(run(ctx)(compose(compose(cat(f), cat(g)), cat(h)))));
  }

} // end of namespace Contextual::Laws
