#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{

  template<typename F, typename T>
  class Application
  {
  public:

    using function_type = F;
    using function_const_reference = function_type const&;
    using function_rvalue_reference = function_type &&;

    using argument_type = T;
    using argument_const_reference = argument_type const&;
    using argument_rvalue_reference = argument_type &&;


    template<typename G, typename U>
    constexpr
    Application(G&& g, U&& x)
      : function_(std::forward<G>(g))
      , argument_(std::forward<U>(x))
    {}

  protected:

    constexpr function_const_reference
    function() const& { return function_; }

    constexpr function_rvalue_reference
    function() && { return std::move(function_); }

    constexpr argument_const_reference
    argument() const& { return argument_; }

    constexpr argument_rvalue_reference
    argument() && { return std::move(argument_); }

    ~Application() = default;

  private:

    function_type function_;
    argument_type argument_;

  }; // end of class Application

} // end of namespace Contextual::Details
