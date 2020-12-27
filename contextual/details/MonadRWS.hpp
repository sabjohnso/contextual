#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Monad.hpp>
#include <contextual/details/MonadReader.hpp>
#include <contextual/details/MonadWriter.hpp>
#include <contextual/details/MonadState.hpp>

namespace Contextual::Details
{
  class MonadRWS : public Monad
  {
  public:
    static constexpr auto ask   = MonadReader::ask;
    static constexpr auto asks  = MonadReader::asks;
    static constexpr auto local = MonadReader::local;

    static constexpr auto writer = MonadWriter::writer;
    static constexpr auto tell   = MonadWriter::tell;
    static constexpr auto listen = MonadWriter::listen;
    static constexpr auto pass   = MonadWriter::pass;
    static constexpr auto censor = MonadWriter::censor;

    static constexpr auto get     = MonadState::get;
    static constexpr auto put     = MonadState::put;
    static constexpr auto select  = MonadState::select;
    static constexpr auto modify  = MonadState::modify;
  };
} // end of namespace Contextual::Details
