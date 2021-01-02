#pragma once

//
// ... Standard header files
//
#include <type_traits>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>

//
// ... Contextual header files
//
#include <contextual/Monad.hpp>

namespace Contextual::Instances
{

  namespace TypeContextDetails
  {
    using std::result_of_t;
    using TypeUtility::type;

    class ProtoTypeContext {
    public:
      static constexpr auto pure =
        []<typename T>(T){ return type<t>; };

      static constexpr auto flatMap =
        []<typename F, typename T>(F, Type<T>){ return result_of_t<F(T)>{}; };

    }; // end of class ProtoTypeContext

    /**
     * @brief TypeContext is a monad context for type proxies
     */
    class TypeContext : public Derive<ProtoTypeContext, MixinMonad, MixinMonadUtility>
    {}; // end of class TypeContext

  } // end of namespace Details
} // end of namespace Contextual::Instances
