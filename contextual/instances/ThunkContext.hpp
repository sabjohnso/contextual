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
#include <contextual/Monad.hpp>

namespace Contextual::Instances
{
  namespace ThunkContextDetails
  {
    using std::forward;
    using TypeUtility::Nat;
    using FunctionUtility::Static_curried;

    class ProtoThunkContext
    {
      class Pure : public Static_curried<Pure, Nat<1>> {
      public:
        template<typename T>
        static constexpr auto
        call(T&& x){
          return [x = forward<T>(x)]{ return x; };
        }
      }; // end of class Pure

      class FlatMap : public Static_curried<FlatMap, Nat<2>> {
      public:
        template<typename F, typename T>
        static constexpr auto
        call(F&& f, T&& mx){
          return [f = forward<F>(f), mx = forward<T>(mx)]{
            return f(mx())();
          };
        }
      }; // end of class FlatMap
    public:
      static constexpr Pure pure{};
      static constexpr FlatMap flatMap{};
    }; // end of class ProtoThunkContext


  } // end of namespace ThunkContextDetails

  class ThunkContext : public Derive<ThunkContextDetails::ProtoThunkContext, MixinMonad>
  {
  public:
    static constexpr auto force = [](auto&& thunk){ return thunk(); };
  };

  namespace ThunkContextNS
  {
    constexpr auto fMap    = ThunkContext::fMap;
    constexpr auto letF    = ThunkContext::letF;
    constexpr auto pure    = ThunkContext::pure;
    constexpr auto returnM = ThunkContext::returnM;
    constexpr auto fApply  = ThunkContext::fApply;
    constexpr auto fApply2 = ThunkContext::fApply2;
    constexpr auto fApply3 = ThunkContext::fApply3;
    constexpr auto fApply4 = ThunkContext::fApply4;
    constexpr auto fApply5 = ThunkContext::fApply5;
    constexpr auto liftM   = ThunkContext::liftM;
    constexpr auto liftM2  = ThunkContext::liftM2;
    constexpr auto liftM3  = ThunkContext::liftM3;
    constexpr auto liftM4  = ThunkContext::liftM4;
    constexpr auto liftM5  = ThunkContext::liftM5;
    constexpr auto flatMap = ThunkContext::flatMap;
    constexpr auto letM    = ThunkContext::letM;
    constexpr auto flatten = ThunkContext::flatten;

    constexpr auto force   = ThunkContext::force;

  } // end of namespace ThunkContextNS
} // end of namespace Contextual::Instances
