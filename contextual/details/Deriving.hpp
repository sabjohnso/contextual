#pragma once

namespace Contextual::Details
{

  template<typename T, template<typename ...> class ... DerivativeMethods>
  class Deriving;

  template<
    typename T,
    template<typename ...> class DerivativeMethod,
    template<typename ...> class ... DerivativeMethods>
  class Deriving<T, DerivativeMethod, DerivativeMethods ...>
    : public DerivativeMethod<T>
    , public Deriving<T,  DerivativeMethods ...>
  {};

  template<typename T>
  class Deriving<T>
  {};
} // end of namespace Contextual::Details
