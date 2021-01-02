#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Applicative.hpp>

namespace Contextual::Details
{

  /**
   * @brief A monoid on applicative functors
   */
  class Alternative : public Applicative
  {};
} // end of namespace Contextual::Details
