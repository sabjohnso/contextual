#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{
  class MonadFail : public Monad
  {
  public:
    template<typename T>
    static constexpr auto fail =
      asksC([]<typename Context>
            (Context){ return Context::template fail<T>(); });
  }; // end of class MonadFail


} // end of namespace Contextual::Details
