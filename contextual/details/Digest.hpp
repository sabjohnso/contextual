#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Functor.hpp>

namespace Contextual::Details
{
  /**
   * @brief Place two layers of idempotent context sensitivity over the input
   * where the inner layer is over values in the context.
   *
   * @details The intent of the `digest` function is to provide the necessary
   * layers of context sensitivity for comonadic context in a manner that is not
   * visible to the user.  The `digest` function requires functor contexts as it
   * relies on `fMap`, but is also only of value for some functorial contexts: those
   * that are comonadic.
   */
  class Digest : public Static_curried<Digest,Nat<1>> {
  public:

    /**
     * @brief return the result of functorial mapping of the `injest`
     * function over the input.
     */
    template<typename T>
    static constexpr auto
    call(T&& mx){return Functor::fMap(injest, mx);}

  } digest{};

} // end of namespace Contextual::Details
