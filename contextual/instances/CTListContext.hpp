#pragma once

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <list_processing/compile_time.hpp>
#include <list_processing/operators.hpp>


//
// ... Contextual header files
//
#include <contextual/Derive.hpp>
#include <contextual/MonadPlus.hpp>
#include <contextual/Monoid.hpp>

namespace Contextual::Instances
{
  namespace CTListContextDetails
  {
    using TypeUtility::Type;

    using ListProcessing::CompileTime::list;
    using ListProcessing::Operators::flatMapList;
    using ListProcessing::Operators::append;
    using ListProcessing::CompileTime::nothing;

    /**
     * @brief Implementation of the methods required for the definition of
     * a context for compile-time lists with the MonadPlus and Monoid methods.
     */
    class ProtoCTListContext {
    public:
      static constexpr auto pure = list;
      static constexpr auto flatMap = flatMapList;
      static constexpr auto mPlus = append;
      static constexpr auto mZero = []<typename T>(Type<T>){ return nothing; };

      static constexpr auto mAppend = append;
      static constexpr auto mEmpty = nothing;
    }; // end of class ProtoCTListContext

  } // end of namespace CTListContextDetails

  /**
   * @brief A Context for Lists  that contains the MonadPlus and Monoid methods
   */
  class CTListContext : public Derive<CTListContextDetails::ProtoCTListContext, MixinMonadPlus, MixinMonoid>
  {} constexpr ctListContext{};


  namespace CTListContextNS
  {
    constexpr auto op       = CTListContext::op;
    constexpr auto op_      = CTListContext::op_;
    constexpr auto mEmpty   = CTListContext::mEmpty;
    constexpr auto mAppend  = CTListContext::mAppend;
    constexpr auto mAppend_ = CTListContext::mAppend_;

    constexpr auto fMap     = CTListContext::fMap;
    constexpr auto letF     = CTListContext::letF;
    constexpr auto pure     = CTListContext::pure;
    constexpr auto returnM  = CTListContext::returnM;
    constexpr auto fApply   = CTListContext::fApply;
    constexpr auto fApply2  = CTListContext::fApply2;
    constexpr auto fApply3  = CTListContext::fApply3;
    constexpr auto fApply4  = CTListContext::fApply4;
    constexpr auto fApply5  = CTListContext::fApply5;
    constexpr auto liftM    = CTListContext::liftM;
    constexpr auto liftM2   = CTListContext::liftM2;
    constexpr auto liftM3   = CTListContext::liftM3;
    constexpr auto liftM4   = CTListContext::liftM4;
    constexpr auto liftM5   = CTListContext::liftM5;
    constexpr auto flatMap  = CTListContext::flatMap;
    constexpr auto letM     = CTListContext::letM;
    constexpr auto flatten  = CTListContext::flatten;
    constexpr auto fail     = CTListContext::fail;
    constexpr auto mZero    = CTListContext::mZero;
    constexpr auto mPlus    = CTListContext::mPlus;

  } // end of namespace CTListContextNS



} // end of namespace Contextual::Instances
