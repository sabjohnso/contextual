#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Monad.hpp>

namespace Contextual
{
  using ::Contextual::Details::Monad;
  using ::Contextual::Details::MixinMonad;
  using ::Contextual::Details::MixinMonadUtility;


  namespace MonadNS
  {
    constexpr auto fMap    = Monad::fMap;
    constexpr auto letF    = Monad::letF;
    constexpr auto pure    = Monad::pure;
    constexpr auto returnM = Monad::returnM;
    constexpr auto fApply  = Monad::fApply;
    constexpr auto fApply2 = Monad::fApply2;
    constexpr auto fApply3 = Monad::fApply3;
    constexpr auto fApply4 = Monad::fApply4;
    constexpr auto fApply5 = Monad::fApply5;
    constexpr auto liftM   = Monad::liftM;
    constexpr auto liftM2  = Monad::liftM2;
    constexpr auto liftM3  = Monad::liftM3;
    constexpr auto liftM4  = Monad::liftM4;
    constexpr auto liftM5  = Monad::liftM5;
    constexpr auto flatMap = Monad::flatMap;
    constexpr auto letM    = Monad::letM;
    constexpr auto flatten = Monad::flatten;

  } // end of namespace MonadNS

} // end of namespace Contextual
