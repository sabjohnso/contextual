#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>

namespace Contextual::Details
{
  /**
   * @brief Place a context independent value into the
   * context sensitive monad or return a context sensitive value,
   * unaltered.
   *
   * @details The `injest` function acts as a idempotent constructor
   * for context sensitive values. The purpose is to provide less convoluted
   * user interaction by hiding the context sensitive monad from the user's
   * view.
   */
  class Injest : public Static_curried<Injest, Nat<1>>{
  public:

    /**
     * @brief Return the input unaltered.
     */
    template<ContextSensitive T>
    static constexpr auto
    call(T&& x){return x;}

    /**
     * @brief Return the input in a context sensitive expression.
     */
    template<typename T>
    static constexpr auto
    call(T&& x){ return returnC(std::forward<T>(x)); }

  } injest{};

} // end of namespace Contextual::Details
