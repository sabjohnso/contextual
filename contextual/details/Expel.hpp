#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Run.hpp>
#include <contextual/details/Contextual.hpp>

namespace Contextual::Details
{
  constexpr
  class Expel : public Static_curried<Expel, Nat<2>>{
  public:
    template<typename Context, typename T>
    static constexpr auto
    call(Context&& ctx, T&& cmx){
      return run(ctx, injest(cmx));
    }
  } expel{};

  constexpr auto askExpel = asksC(expel);

} // end of namespace Contextual::Details
