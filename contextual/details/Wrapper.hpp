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
    Wrapper(rvalue_reference input) : value_(std::move(input)){}


    constexpr const_reference
    value() const& {return value_;}

    constexpr rvalue_reference
    value() && {return std::move(value_);}

  protected:

    ~Wrapper() = default;

  private:
    value_type value_;

  }; // end of class Wrapper


} // end of namespace Contextual::Details
