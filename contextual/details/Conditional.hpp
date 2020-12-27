#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Either.hpp>

namespace Contextual::Details
{

  template<typename Test, typename Pass, typename Fail>
  class Conditional
  {
  public:

    using test_type = Test;
    using pass_type = Pass;
    using fail_type = Fail;

    template<typename T, typename P, typename F>
    constexpr
    Conditional(T&& test, P&& pass, F&& fail)
      : test_(forward<T>(test))
      , pass_(forward<P>(pass))
      , fail_(forward<F>(fail))
    {}

    template<
      typename Context,
      typename PassResult = result_of_t<Run(Context,Pass)>,
      typename FailResult = result_of_t<Run(Context,Fail)>,
      typename Result = conditional_t<
        is_same_v<PassResult,FailResult>,
        PassResult, Either<PassResult,FailResult>
        >
      >
    constexpr Result
    operator ()(Context&& ctx) const& {
      return test_
        ? Result(run(forward<Context>(ctx), pass_))
        : Result(run(forward<Context>(ctx), fail_));
    }

    template<
      typename Context,
      typename PassResult = result_of_t<Run(Context,Pass)>,
      typename FailResult = result_of_t<Run(Context,Fail)>,
      typename Result = conditional_t<
        is_same_v<PassResult,FailResult>,
        PassResult, Either<PassResult,FailResult>
        >
      >
    constexpr Result
    operator ()(Context&& ctx) && {
      return test_
        ? Result(run(forward<Context>(ctx), move(pass_)))
        : Result(run(forward<Context>(ctx), move(fail_)));
    }

  private:
    test_type test_;
    pass_type pass_;
    fail_type fail_;
  };

  template<typename T, typename P, typename F>
  Conditional(T&& test, P&& pass, F&& fail) -> Conditional<decay_t<T>, decay_t<P>, decay_t<F>>;

  constexpr
  class Cond : public Static_callable<Cond> {
    template<typename T, typename P, typename F>
    static constexpr auto
    call(T&& test, P&& pass, F&& fail){
      return Conditional(
        forward<T>(test),
        forward<P>(pass),
        forward<F>(fail));
    }

    template<typename T1, typename P1, typename T2, typename P2, typename Next, typename ... More>
    static constexpr auto
    call(T1&& test1, P1&& pass1, T2&& test2, P2&& pass2, Next&& next, More&& ... more){
      return Conditional(
        forward<T1>(test1),
        forward<P1>(pass1),
        call(forward<T2>(test2), forward<P2>(pass2), forward<Next>(next), forward<More>(more) ...));
    }
  } cond{};


} // end of namespace Contextual::Details
