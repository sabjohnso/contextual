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

    constexpr
    class FunctionContext : public Deriving<
      FunctionContext,
      MonadReader::MonadicFMap,
      MonadReader::MonadicFApply,
      MonadReader::MonadicFlatten
      >
    {
    public:

      template<typename T>
      static constexpr auto
      pure(T x){ return constant(x); }

      template<typename F, typename T>
      static constexpr auto
      flatMap(F f, T mx){
        return [=](auto e){ return f(mx(e))(e); };
      }

      static constexpr auto
      ask(){ return identity; }

      template<typename F, typename T>
      static constexpr auto
      local(F f, T mx){
        return [=](auto env){ return mx(f(env)); };
      }
    } functionContext{};

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
