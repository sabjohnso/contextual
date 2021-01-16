#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Monoid.hpp>

namespace Contextual
{
  using Details::Monoid;

  using Details::HasMAppend;
  using Details::MissingMAppend;

  using Details::HasMEmpty;
  using Details::MissingMEmpty;

  using Details::HasMinimalMonoid;
  using Details::MissingMinimalMonoid;

  using Details::HasMonoid;
  using Details::MissingMonoid;

  using Details::MixinMonoid;

} // end of namespace Contextual
