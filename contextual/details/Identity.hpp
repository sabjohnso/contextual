#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  constexpr
  class Identity : public Static_curried<Identity, Nat<1>> {
  public:
    template<typename T>
    static constexpr auto
    call(T&& x){ return x; }
  } identity{};

} // end of namespace Contextual::Details
