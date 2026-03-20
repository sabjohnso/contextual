//
// ... Standard header files
//
#include <functional>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Semigroup.hpp>
#include "laws/SemigroupLaws.hpp"

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::plus;
    using std::multiplies;

    class ProtoAdditionSemigroup
    {
    public:
      static constexpr auto op = curry<2>(plus{});
    };

    class AdditionSemigroup : public Derive<ProtoAdditionSemigroup, MixinSemigroup>
    {} constexpr additionSemigroup{};

    class ProtoMultiplicationSemigroup
    {
    public:
      static constexpr auto op = curry<2>(multiplies{});
    };

    class MultiplicationSemigroup
      : public Derive<ProtoMultiplicationSemigroup, MixinSemigroup>
    {} constexpr multiplicationSemigroup{};

  } // end of anonymous namespace

  RC_GTEST_PROP(SemigroupLaws, AdditionAssociativity, (int a, int b, int c))
  {
    Laws::semigroupAssociativity(additionSemigroup, a, b, c);
  }

  RC_GTEST_PROP(SemigroupLaws, MultiplicationAssociativity, (int a, int b, int c))
  {
    Laws::semigroupAssociativity(multiplicationSemigroup, a, b, c);
  }

} // end of namespace Contextual::Details::Testing
