#pragma once
//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Functor.hpp>
#include <contextual/details/Digest.hpp>

namespace Contextual::Details
{

  /**
   * @brief Functor mapping with two layers of context sensitivity.
   */
  class CofMap : public Static_curried<CoFMap, Nat<2>> {
  public:
    template<typename F, typename T>
    static constexpr auto
    call(F&& f, T&& cmcx){
      return
        letC(askC, [=](auto ctx ){ return
        letC(asksC(getFMap), [=](auto fmap){ return
        letC(fMap([=](auto cx){ return run(ctx, cx); }, digest(cmcmx)), [=](auto mx){ return
              digest(fMap(f, mx)); }); }); });
    }
  } cofMap{};

} // end of namespace Contextual::Details
