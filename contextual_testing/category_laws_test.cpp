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
// ... External header files
//
#include <function_utility/function_utility.hpp>

//
// ... Contextual header files
//
#include <contextual/details/Run.hpp>
#include <contextual/details/Category.hpp>
#include "laws/CategoryLaws.hpp"

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    class FunctionContext
    {
    public:
      template<typename F>
      static constexpr auto
      cat(F f) { return f; }

      template<typename F, typename G>
      static constexpr auto
      compose(F f, G g)
      {
        return [=](auto x) { return f(g(x)); };
      }

      static constexpr auto identity = [](auto x) { return x; };
    };

    constexpr FunctionContext functionContext{};

    auto twc = [](int x) { return x + x; };
    auto sqr = [](int x) { return x * x; };
    auto inc = [](int x) { return x + 1; };

  } // end of anonymous namespace

  RC_GTEST_PROP(CategoryLaws, LeftIdentity, (int x))
  {
    Laws::categoryLeftIdentity(functionContext, twc, x);
  }

  RC_GTEST_PROP(CategoryLaws, RightIdentity, (int x))
  {
    Laws::categoryRightIdentity(functionContext, sqr, x);
  }

  RC_GTEST_PROP(CategoryLaws, Associativity, (int x))
  {
    Laws::categoryAssociativity(functionContext, twc, sqr, inc, x);
  }

} // end of namespace Contextual::Details::Testing
