#pragma once

//
// ... Standard header files
//
#include <functional>

//
// ... External header files
//
#include <function_utility/function_utility.hpp>
#include <ctstring/ctstring.hpp>

//
// ... Contextual header files
//
#include <contextual/Derive.hpp>
#include <contextual/Monoid.hpp>

namespace Contextual::Instances
{
  namespace CTStringContextDetails
  {
    using std::plus;
    using FunctionUtility::curry;
    using CTString::hoist;
    using CTString::static_string;

    class ProtoCTStringContext
    {
    public:
      static constexpr auto mEmpty = hoist([]{ return static_string(""); });
      static constexpr auto mAppend = curry<2>(plus{});
    }; // end of class ProtoCTStringContext
    static_assert(HasMinimalMonoid<ProtoCTStringContext>);
  } // end of namespace CTStringContextDetails

  class CTStringContext : public Derive<CTStringContextDetails::ProtoCTStringContext, MixinMonoid>
  {}; // end of class CTStringContext

  namespace CTStringContextNS
  {
    constexpr auto op       = CTStringContext::op;
    constexpr auto op_      = CTStringContext::op_;
    constexpr auto mEmpty   = CTStringContext::mEmpty;
    constexpr auto mAppend  = CTStringContext::mAppend;
    constexpr auto mAppend_ = CTStringContext::mAppend_;
  }


} // end of namespace Contextual::Instances
