#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Semigroup.hpp>

namespace Contextual::Details
{

  class Monoid : public Semigroup
  {
    using base = Semigroup;
  public:
    static constexpr auto mempty =
      asksC([]<typename Context>(Context){ return Context::mempty; });

    static constexpr auto mappend = base::op;
    static constexpr auto mappend_ = base::op_;

  }; // end of class Monoid

} // end of namespace Contextual::Details
