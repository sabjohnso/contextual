#pragma once

namespace Contextual::Details
{
  constexpr struct Unit{

    friend constexpr bool
    operator ==(Unit, Unit){ return true; }

    friend constexpr bool
    operator !=(Unit x, Unit y){ return !(x == y); }

  } unit{};
} // end of namespace Contextual::Details
