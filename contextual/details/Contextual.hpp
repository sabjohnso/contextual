#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Wrapper.hpp>
#include <contextual/details/Application.hpp>
#include <contextual/details/Run.hpp>


namespace Contextual::Details
{
  template<typename F>
  class Contextual
  {

    friend class Run;

    using function_type = F;
    using function_const_reference = function_type const&;
    using function_rvalue_reference = function_type &&;

  public:
    constexpr
    Contextual(function_const_reference function)
      : function_(function)
    {}

    constexpr
    Contextual(function_rvalue_reference function)
      : function_(move(function))
    {}


    template<typename Context>
    constexpr auto
    operator()(Context&& ctx) const& { return function_(forward<Context>(ctx)); }

    template<typename Context>
    constexpr auto
    operator()(Context&& ctx) && { return move(function_)(forward<Context>(ctx)); }

  private:

    function_type function_;
  }; // end of class Contextual

  template<typename F>
  Contextual(F&& function) -> Contextual<decay_t<F>>;

  template<typename T>
  struct IsContextual : false_type {};

  template<typename F>
  struct IsContextual<Contextual<F>> : true_type{};

  template<typename F>
  struct IsContextual<Contextual<F> &> : true_type{};

  template<typename F>
  struct IsContextual<Contextual<F> &&> : true_type{};

  template<typename F>
  struct IsContextual<Contextual<F> const&> : true_type{};

  template<typename T>
  concept ContextSensitive = IsContextual<T>::value;


  template<typename T>
  class ContextualReturn : Wrapper<T>
  {
  public:
    using base = Wrapper<T>;
    using const_reference = typename base::const_reference;
    using rvalue_reference = typename base::rvalue_reference;

    using base::base;

    template<typename Context>
    constexpr const_reference
    operator ()(Context &&) const& { return base::value(); }

    template<typename Context>
    constexpr const_reference
    operator ()(Context &&) && { return base::value(); }

  }; // end of class ContextualReturn

  template<typename T>
  ContextualReturn(T&& x) -> ContextualReturn<decay_t<T>>;


  constexpr
  class ReturnC : public Static_curried<ReturnC, Nat<1>>
  {
  public:
    template<typename T>
    static constexpr auto
    call(T&& x){ return Contextual(ContextualReturn(forward<T>(x))); }
  } returnC{};


  template<typename F, ContextSensitive T>
  class ContextualFMap : Application<F,T>
  {
  public:
    using base = Application<F,T>;
    using base::base;

    template<typename Context>
    constexpr auto
    operator ()(Context&& ctx) const& {
      return base::function()(run(forward<Context>(ctx), base::argument()));
    }

    template<typename Context>
    constexpr auto
    operator ()(Context&& ctx) && {
      return base::function()(run(forward<Context>(ctx), base::argument()));
    }
  };

  template<typename F, typename T>
  ContextualFMap(F&& f, T&& mx) -> ContextualFMap<decay_t<F>, decay_t<T>>;

  constexpr
  class FMapC : public Static_curried<FMapC, Nat<2>>
  {
  public:
    template<typename F, ContextSensitive T>
    static constexpr auto
    call(F&& f, T&& mx){
      return Contextual(ContextualFMap(forward<F>(f), forward<T>(mx)));
    }
  } fmapC{};


  template<typename F, typename T>
  class ContextualFlatMap : Application<F,T>
  {
  public:
    using base = Application<F,T>;
    using base::base;

    template<typename Context>
    constexpr auto
    operator()(Context&& ctx) const& {
      return run(ctx, base::function()(run( ctx, base::argument())));
    }

    template<typename Context>
    constexpr auto
    operator()(Context&& ctx) && {
      return run(ctx, base::function()(run( ctx, base::argument())));
    }
  }; // end of class ContextualFlatMap

  template<typename F, typename T>
  ContextualFlatMap(F&& f, T&& mx) -> ContextualFlatMap<decay_t<F>, decay_t<T>>;

  class FlatMapC : public Static_curried<FlatMapC, Nat<2>>
  {
  public:
    template<typename F, typename T>
    static constexpr auto
    call(F&& f, T&& mx){ return Contextual(ContextualFlatMap(forward<F>(f), forward<T>(mx))); }
  } flatMapC{};

  constexpr auto letC =
    [](auto&& mx, auto&& f){
      return flatMapC(forward<decltype(f)>(f),
                      forward<decltype(mx)>(mx));
    };


  class ContextualAsk
  {
  public:
    template<typename Context>
    constexpr auto
    operator()(Context&& ctx) const { return ctx; }
  };

  constexpr auto askC = Contextual{ContextualAsk{}};


  class AsksC : public Static_curried<AsksC,Nat<1>>{
  public:
    template<typename F>
    static constexpr auto
    call( F&& function){ return fmapC(forward<F>(function), askC); }
  } asksC{};

  static constexpr auto curry2 =
    []<typename F>
    (F&& f){return curry(forward<F>(f), nat<2>);
  };

  static constexpr auto curry3 =
    []<typename F>
    (F&& f){return curry(forward<F>(f), nat<3>);
  };

  constexpr auto asksC2 = compose(asksC, curry2);
  constexpr auto asksC3 = compose(asksC, curry3);




} // end of namespace Contextual::Details
