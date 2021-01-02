//
// ... Standard header files
//
#include <functional>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Constant.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/MonadReader.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::plus;

    constexpr auto ask   = MonadReader::ask;
    constexpr auto asks  = MonadReader::asks;
    constexpr auto local = MonadReader::local;

    class ProtoFunctionContext
    {
    public:

      static constexpr auto ask = identity;
      static constexpr auto pure = constant;
      static constexpr auto flatMap =
        curry<3>([](auto f, auto mx, auto e){
          return f(mx(e))(e);
        });

      static constexpr auto local =
        curry<3>([](auto f, auto mx, auto e){
          return mx(f(e));
        });
    };

    class FunctionContext : public Derive<ProtoFunctionContext, MixinMonadReader>
    {} constexpr functionContext{};

  } // end of anonymous namespace

  TEST(MonadReader, FunctionAsk){
    EXPECT_EQ(run(3)( run(functionContext, ask)), 3);
  }

  TEST(MonadReader, FunctionAsks){
    EXPECT_EQ(run(3, run(functionContext, asks([](auto x){ return x+1; }))), 4 );
  }

  TEST(MonadReader, FunctionLocal){
    constexpr auto add = curry<2>(plus{});
    EXPECT_EQ(run(3, run(functionContext, local(add(1), add(4)))), 8);
  }

} // end of namespace Contextual::Details::Testing
