#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  constexpr
  class Constant : public Static_curried<Constant,Nat<2>>
  {
  public:
    template<typename T, typename U>
    static constexpr auto
    call(T&& x, U&&){ return x; }
  } constant{};

} // end of namespace Contextual::Details
