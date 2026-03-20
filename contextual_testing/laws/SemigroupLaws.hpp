#pragma once

#include <rapidcheck.h>
#include <contextual/details/Run.hpp>
#include <contextual/details/Semigroup.hpp>

namespace Contextual::Laws
{
  using Details::run;

  constexpr auto op = Details::Semigroup::op;

  /**
   * @brief Semigroup associativity: op(op(x, y), z) == op(x, op(y, z))
   */
  template<typename Context, typename T>
  void semigroupAssociativity(Context ctx, T x, T y, T z)
  {
    RC_ASSERT(
      run(ctx)(op(op(x, y), z)) ==
      run(ctx)(op(x, op(y, z))));
  }

} // end of namespace Contextual::Laws
