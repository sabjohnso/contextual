#pragma once

#include <rapidcheck.h>
#include <contextual/details/Run.hpp>
#include <contextual/details/Monoid.hpp>

namespace Contextual::Laws
{
  using Details::run;

  constexpr auto mAppend = Details::Monoid::mAppend;
  constexpr auto mEmpty  = Details::Monoid::mEmpty;

  /**
   * @brief Monoid left identity: mAppend(mEmpty, x) == x
   */
  template<typename Context, typename T>
  void monoidLeftIdentity(Context ctx, T x)
  {
    RC_ASSERT(run(ctx)(mAppend(mEmpty, x)) == x);
  }

  /**
   * @brief Monoid right identity: mAppend(x, mEmpty) == x
   */
  template<typename Context, typename T>
  void monoidRightIdentity(Context ctx, T x)
  {
    RC_ASSERT(run(ctx)(mAppend(x, mEmpty)) == x);
  }

  /**
   * @brief Monoid associativity: mAppend(mAppend(x, y), z) == mAppend(x, mAppend(y, z))
   */
  template<typename Context, typename T>
  void monoidAssociativity(Context ctx, T x, T y, T z)
  {
    RC_ASSERT(
      run(ctx)(mAppend(mAppend(x, y), z)) ==
      run(ctx)(mAppend(x, mAppend(y, z))));
  }

} // end of namespace Contextual::Laws
