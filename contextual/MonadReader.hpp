#pragma once

//
// ... Contextual header files
//
#include <contextual/details/MonadReader.hpp>

namespace Contextual
{
  using ::Contextual::Details::HasMinimalMonadReader;
  using ::Contextual::Details::HasMonadReader;
  using ::Contextual::Details::MonadReader;
  using ::Contextual::Details::MixinMonadReader;

  namespace MonadReaderNS
  {

    constexpr auto fMap    = MonadReader::fMap;
    constexpr auto letF    = MonadReader::letF;
    constexpr auto pure    = MonadReader::pure;
    constexpr auto returnM = MonadReader::returnM;
    constexpr auto fApply  = MonadReader::fApply;
    constexpr auto fApply2 = MonadReader::fApply2;
    constexpr auto fApply3 = MonadReader::fApply3;
    constexpr auto fApply4 = MonadReader::fApply4;
    constexpr auto fApply5 = MonadReader::fApply5;
    constexpr auto liftM   = MonadReader::liftM;
    constexpr auto liftM2  = MonadReader::liftM2;
    constexpr auto liftM3  = MonadReader::liftM3;
    constexpr auto liftM4  = MonadReader::liftM4;
    constexpr auto liftM5  = MonadReader::liftM5;
    constexpr auto flatMap = MonadReader::flatMap;
    constexpr auto letM    = MonadReader::letM;
    constexpr auto flatten = MonadReader::flatten;

    constexpr auto ask     = MonadReader::ask;
    constexpr auto asks    = MonadReader::asks;
    constexpr auto local   = MonadReader::local;

  } // end of namespace MonadReaderNS
} // end of namespace Contextual
