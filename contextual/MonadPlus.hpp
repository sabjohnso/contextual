#pragma once

//
// ... Contextual header files
//
#include <contextual/details/MonadPlus.hpp>

namespace Contextual
{
  using ::Contextual::Details::MonadPlus;
  using ::Contextual::Details::MixinMonadPlus;
  using ::Contextual::Details::MixinMonadPlusUtility;

  namespace MonadPlusNS
  {

    constexpr auto fMap    = MonadPlus::fMap;
    constexpr auto letF    = MonadPlus::letF;
    constexpr auto pure    = MonadPlus::pure;
    constexpr auto returnM = MonadPlus::returnM;
    constexpr auto fApply  = MonadPlus::fApply;
    constexpr auto fApply2 = MonadPlus::fApply2;
    constexpr auto fApply3 = MonadPlus::fApply3;
    constexpr auto fApply4 = MonadPlus::fApply4;
    constexpr auto fApply5 = MonadPlus::fApply5;
    constexpr auto liftM   = MonadPlus::liftM;
    constexpr auto liftM2  = MonadPlus::liftM2;
    constexpr auto liftM3  = MonadPlus::liftM3;
    constexpr auto liftM4  = MonadPlus::liftM4;
    constexpr auto liftM5  = MonadPlus::liftM5;
    constexpr auto flatMap = MonadPlus::flatMap;
    constexpr auto letM    = MonadPlus::letM;
    constexpr auto flatten = MonadPlus::flatten;
    constexpr auto mZero   = MonadPlus::mZero;
    constexpr auto mPlus   = MonadPlus::mPlus;

  } // end of namespace MonadPlusNS

} // end of namespace Contextual
