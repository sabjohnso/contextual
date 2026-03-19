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
   * @details The `ingest` function acts as an idempotent constructor
   * for context sensitive values. The purpose is to provide less convoluted
   * user interaction by hiding the context sensitive monad from the user's
   * view.
   */
  class Ingest : public Static_curried<Ingest, Nat<1>>{
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

  } ingest{};

} // end of namespace Contextual::Details
