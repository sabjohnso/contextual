#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  template<typename T>
  class Wrapper
  {
  public:

    using value_type = T;
    using const_reference = value_type const&;
    using rvalue_reference = value_type &&;

    constexpr
    Wrapper(const_reference input) : value_(input){}

    constexpr
    Wrapper(rvalue_reference input) : value_(move(input)){}


  protected:

    const_reference
    value() const& {return value_;}

    rvalue_reference
    value() && {return move(value_);}

    ~Wrapper() = default;

  private:
    value_type value_;

  }; // end of class Wrapper


} // end of namespace Contextual::Details
