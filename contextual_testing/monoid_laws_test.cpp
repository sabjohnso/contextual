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
#include <contextual/details/Monoid.hpp>
#include "laws/MonoidLaws.hpp"

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::plus;
    using std::multiplies;

    class ProtoAddition
    {
    public:
      static constexpr int mEmpty = 0;
      static constexpr auto mAppend = curry<2>(plus{});
    };

    class Addition : public Derive<ProtoAddition, MixinMonoid>
    {} constexpr addition{};

    class ProtoMultiplication
    {
    public:
      static constexpr int mEmpty = 1;
      static constexpr auto mAppend = curry<2>(multiplies{});
    };

    class Multiplication : public Derive<ProtoMultiplication, MixinMonoid>
    {} constexpr multiplication{};

  } // end of anonymous namespace

  RC_GTEST_PROP(MonoidLaws, AdditionLeftIdentity, (int x))
  {
    Laws::monoidLeftIdentity(addition, x);
  }

  RC_GTEST_PROP(MonoidLaws, AdditionRightIdentity, (int x))
  {
    Laws::monoidRightIdentity(addition, x);
  }

  RC_GTEST_PROP(MonoidLaws, AdditionAssociativity, (int a, int b, int c))
  {
    Laws::monoidAssociativity(addition, a, b, c);
  }

  RC_GTEST_PROP(MonoidLaws, MultiplicationLeftIdentity, (int x))
  {
    Laws::monoidLeftIdentity(multiplication, x);
  }

  RC_GTEST_PROP(MonoidLaws, MultiplicationRightIdentity, (int x))
  {
    Laws::monoidRightIdentity(multiplication, x);
  }

  RC_GTEST_PROP(MonoidLaws, MultiplicationAssociativity, (int a, int b, int c))
  {
    Laws::monoidAssociativity(multiplication, a, b, c);
  }

} // end of namespace Contextual::Details::Testing
