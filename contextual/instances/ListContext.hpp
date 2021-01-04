#pragma once

//
// ... Standard header files
//
#include <utility>
#include <type_traits>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/Derive.hpp>
#include <contextual/MonadPlus.hpp>

namespace Contextual::Instances
{
  namespace ListContextDetails
  {
    using std::result_of_t;
    using std::forward;

    using TypeUtility::Nat;
    using TypeUtility::Type;

    using FunctionUtility::Static_curried;
    using FunctionUtility::Trampoline;

    using ListProcessing::Dynamic::list;
    using ListProcessing::Dynamic::nil;
    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::ListType;

    class ProtoListContext{

      class FMap : public Static_curried<FMap, Nat<2>>{
      public:
        template<typename F, typename T>
        static auto
        call(F&& f, T&& xs){
          return  map(forward<F>(f), forward<T>(xs));
        }

      }; // end of class FMap

      class Pure : public Static_curried<Pure, Nat<1>> {
      public:
        template<typename T>
        static auto
        call(T&& x){ return list(forward<T>(x)); }
      }; // end of class Pure


      class FApply : public Static_curried<FApply, Nat<2>> {
      public:
        template<typename F, typename T>
        static auto
        call(F fs, T xs){
          using Result = ListType<decltype(head(fs)(head(xs)))>;
          return aux(fs, xs, Result::nil);
        }
      private:
        template<typename F, typename T, typename Result>
        static Result
        aux(F fs, T xs, Result accum){
          return hasData(fs)
            ? aux(tail(fs), xs, rappend(map(head(fs), xs), accum))
            : reverse(accum);
        }
      }; // end of class FApply


      class FlatMap : public Static_curried<FlatMap, Nat<2>>{
      public:
        template<typename F, typename T>
        static auto
        call(F&& f, T xs){  return mMap(forward<F>(f), xs); }
      }; // end of class FlatMap

      class Flatten : public Static_curried<Flatten, Nat<1>>{
      public:
        template<typename T, typename R = typename T::value_type>
        static R
        call(T xss){
          return aux(xss, R::nil);
        }

      private:
        template<typename T, typename R>
        static R
        aux(T xss, R accum){
          return hasData(xss)
            ? aux(tail(xss), rappend(head(xss), accum))
            : reverse(accum);
        }
      }; // end of class Flatten

    public:

      static constexpr auto mEmpty = Nil{};
      static constexpr auto mAppend = [](auto xs, auto ys){ return append(xs, ys); };

      static constexpr FMap fMap{};
      static constexpr Pure pure{};
      static constexpr FApply fApply{};
      static constexpr FlatMap flatMap{};
      static constexpr Flatten flatten{};

      static constexpr auto mZero = []<typename T>(Type<T>){ return nil<T>; };
      static constexpr auto mPlus = mAppend;

    }; // end of class Pro
  } // end of namespace ListContextDetails


  class ListContext : public Derive<ListContextDetails::ProtoListContext, MixinMonadPlus, MixinMonoid>
  {};


  namespace ListContextNS
  {

    constexpr auto op       = ListContext::op;
    constexpr auto op_      = ListContext::op_;
    constexpr auto mEmpty   = ListContext::mEmpty;
    constexpr auto mAppend  = ListContext::mAppend;
    constexpr auto mAppend_ = ListContext::mAppend_;

    constexpr auto fMap     = ListContext::fMap;
    constexpr auto letF     = ListContext::letF;
    constexpr auto pure     = ListContext::pure;
    constexpr auto returnM  = ListContext::returnM;
    constexpr auto fApply   = ListContext::fApply;
    constexpr auto fApply2  = ListContext::fApply2;
    constexpr auto fApply3  = ListContext::fApply3;
    constexpr auto fApply4  = ListContext::fApply4;
    constexpr auto fApply5  = ListContext::fApply5;
    constexpr auto liftM    = ListContext::liftM;
    constexpr auto liftM2   = ListContext::liftM2;
    constexpr auto liftM3   = ListContext::liftM3;
    constexpr auto liftM4   = ListContext::liftM4;
    constexpr auto liftM5   = ListContext::liftM5;
    constexpr auto flatMap  = ListContext::flatMap;
    constexpr auto letM     = ListContext::letM;
    constexpr auto flatten  = ListContext::flatten;
    constexpr auto fail     = ListContext::fail;
    constexpr auto mZero    = ListContext::mZero;
    constexpr auto mPlus    = ListContext::mPlus;

  } // end of namespace ListContextNS

} // end of namespace Contextual::Instances
