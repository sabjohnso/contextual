#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  template<typename First, typename Second>
  class Either : variant<First,Second>
  {
  public:
    using first_type = First;
    using second_type = Second;
    using base = variant<First,Second>;
    using base::base;

    Either() = delete;

    constexpr bool
    holdsFirst() const { return holds_alternative<First>(*this); }

    constexpr bool
    holdsSecond() const { return holds_alternative<Second>(*this); }

    constexpr first_type
    first() const { return std::get<first_type>(*this); }

    constexpr second_type
    second() const { return std::get<second_type>(*this); }

  }; // end of class Either

} // end of namespace Contextual::Details
