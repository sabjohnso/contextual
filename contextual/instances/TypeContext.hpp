#pragma once

//
// ... Standard header files
//
#include <type_traits>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>

//
// ... Contextual header files
//
#include <contextual/Monad.hpp>
#include <contextual/Derive.hpp>

namespace Contextual::Instances
{

  namespace TypeContextDetails
  {

    using std::result_of_t;
    using TypeUtility::type;
    using TypeUtility::Type;

    class ProtoTypeContext {
    public:
      static constexpr auto pure =
        []<typename T>(T){ return type<T>; };

      static constexpr auto flatMap =
        []<typename F, typename T>(F, Type<T>){ return result_of_t<F(T)>{}; };

    }; // end of class ProtoTypeContext
  } // end of namespace Details

  /**
   * @brief TypeContext is a monad context for type proxies
   */
  class TypeContext : public Derive<TypeContextDetails::ProtoTypeContext, MixinMonad>
  {} constexpr typeContext{}; // end of class TypeContext

  namespace TypeContextNS
  {
    constexpr auto fMap    = TypeContext::fMap;
    constexpr auto letF    = TypeContext::letF;
    constexpr auto pure    = TypeContext::pure;
    constexpr auto returnM = TypeContext::returnM;
    constexpr auto fApply  = TypeContext::fApply;
    constexpr auto fApply2 = TypeContext::fApply2;
    constexpr auto fApply3 = TypeContext::fApply3;
    constexpr auto fApply4 = TypeContext::fApply4;
    constexpr auto fApply5 = TypeContext::fApply5;
    constexpr auto liftM   = TypeContext::liftM;
    constexpr auto liftM2  = TypeContext::liftM2;
    constexpr auto liftM3  = TypeContext::liftM3;
    constexpr auto liftM4  = TypeContext::liftM4;
    constexpr auto liftM5  = TypeContext::liftM5;
    constexpr auto flatMap = TypeContext::flatMap;
    constexpr auto letM    = TypeContext::letM;
    constexpr auto flatten = TypeContext::flatten;

  } // end of namespace TypeContextNs
} // end of namespace Contextual::Instances
