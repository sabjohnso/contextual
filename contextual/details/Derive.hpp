#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>

namespace Contextual::Details
{
  template<typename T>
  struct IsTypeProxy : false_type{};

  template<typename T>
  struct IsTypeProxy<Type<T>> : true_type{};

  template<typename T>
  concept TypeProxy = IsTypeProxy<T>::value;



  template<typename F, typename T>
  using TypeResult = typename invoke_result_t<F,Type<T>>::type;

  template<typename Proto, typename ... Fs>
  class Derive;

  template<typename Proto>
  class Derive<Proto> : public Proto
  {
  public:
    using Proto::Proto;
  };

  template<typename Proto, typename F, typename ... Fs>
  class Derive<Proto,F , Fs...> : public Derive<TypeResult<F, Proto>, Fs ...>
  {
  public:
    using Base = Derive<TypeResult<F, Proto>, Fs ...>;
    using Base::Base;
  };

} // end of namespace Contextual::Details
