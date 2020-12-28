#pragma once

//
// ... Standard header files
//
#include <type_traits>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>

namespace Contextual::Instances
{


  class TypeContext
  {
    template<typename T>
    using Type = TypeUtility::Type<T>;

    template<typename T>
    static constexpr auto type = TypeUtility::type<T>;

  public:
    static constexpr auto pure =
      curry<1>([]<typename T>(T&& x){ return type<decay_t<T>>; });

    static constexpr auto flatMap =
      curry<2>([]<typename F, typename T>(F&&, Type<T>){
          return type<std::decay_t<std::result_of_t<F(T)>>>;
        });
  };

} // end of namespace Contextual::Instances
