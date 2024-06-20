#pragma once

//
// ... Standard header files
//
#include <utility>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>


//
// ... Contextual header files
//
#include <contextual/Derive.hpp>
#include <contextual/MonadReader.hpp>

namespace Contextual::Instances
{

  namespace FunctionContextDetails
  {
    using std::forward;

    using TypeUtility::Nat;

    using FunctionUtility::constant;
    using FunctionUtility::identity;
    using FunctionUtility::Static_curried;

    class ProtoFunctionContext
    {
      class FlatMap : public Static_curried<FlatMap, Nat<3>> {
      public:
        template<typename F, typename T, typename E>
        static constexpr auto
        call(F&& f, T&& mx, E&& e){
          return std::forward<F>(f)(std::forward<T>(mx)(e))(e);
        }
      }; // end of class FlatMap;

      class Local : public Static_curried<Local, Nat<3>> {
      public:
        template<typename F, typename T, typename E>
        static constexpr auto
        call(F&& f, T&& mx, E&& e){
          return forward<T>(mx)(forward<F>(f)(forward<E>(e)));
        }
      }; // end of class Local

    public:
      static constexpr auto ask = identity;
      static constexpr auto pure = constant;
      static constexpr FlatMap flatMap{};
      static constexpr Local local{};

    }; // end of class ProtoFunctionContext

    static_assert(HasMinimalMonadReader<ProtoFunctionContext>);
  } // end of namespace FunctionContextDetails

  class FunctionContext : public Derive<FunctionContextDetails::ProtoFunctionContext, MixinMonadReader>
  {};

  namespace FunctionContextNS
  {

    constexpr auto fMap    = FunctionContext::fMap;
    constexpr auto letF    = FunctionContext::letF;
    constexpr auto pure    = FunctionContext::pure;
    constexpr auto returnM = FunctionContext::returnM;
    constexpr auto fApply  = FunctionContext::fApply;
    constexpr auto fApply2 = FunctionContext::fApply2;
    constexpr auto fApply3 = FunctionContext::fApply3;
    constexpr auto fApply4 = FunctionContext::fApply4;
    constexpr auto fApply5 = FunctionContext::fApply5;
    constexpr auto liftM   = FunctionContext::liftM;
    constexpr auto liftM2  = FunctionContext::liftM2;
    constexpr auto liftM3  = FunctionContext::liftM3;
    constexpr auto liftM4  = FunctionContext::liftM4;
    constexpr auto liftM5  = FunctionContext::liftM5;
    constexpr auto flatMap = FunctionContext::flatMap;
    constexpr auto letM    = FunctionContext::letM;
    constexpr auto flatten = FunctionContext::flatten;

    constexpr auto ask     = FunctionContext::ask;
    constexpr auto asks    = FunctionContext::asks;
    constexpr auto local   = FunctionContext::local;

  } // end of namespace FunctionContextNS
} // end of namespace Contextual::Instances
