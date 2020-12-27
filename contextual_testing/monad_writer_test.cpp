//
// ... Standard header files
//
#include <utility>
#include <string>

//
// ... Testing header files
//
#include <gtest/gtest.h>


//
// ... External header files
//
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/Unit.hpp>
#include <contextual/details/Monoid.hpp>
#include <contextual/details/MonadWriter.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::pair;
    using std::string;

    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::list;

    constexpr auto mempty = Monoid::mempty;
    constexpr auto mappend = Monoid::mappend;

    constexpr auto pure = MonadWriter::pure;
    constexpr auto writer = MonadWriter::writer;
    constexpr auto tell = MonadWriter::tell;
    constexpr auto listen = MonadWriter::listen;
    constexpr auto pass = MonadWriter::pass;
    constexpr auto censor = MonadWriter::censor;
    constexpr auto returnM = MonadWriter::returnM;
    constexpr auto beginM = MonadWriter::beginM;
    constexpr auto letF = MonadWriter::letF;
    constexpr auto letM = MonadWriter::letM;

    class ListMonoid {
    public:
      static constexpr auto mempty = Nil{};

      template<typename List>
      static constexpr auto
      op(List xs, List ys){
        return append(xs, ys);
      }

      template<typename List>
      static constexpr auto
      op(List xs, Nil){
        return xs;
      }

      template<typename List>
      static constexpr auto
      op(Nil, List xs){
        return xs;
      }
    };

    template<typename LogContext >
    class LoggingContext {
    public:

      static constexpr LogContext logContext{};

      template<typename T>
      static auto
      pure(T x){
        return pair(x, run(logContext, mempty));
      }

      template<typename F, typename T>
      static  auto
      fMap(F f, T mx){
        return pair(f(mx.first), mx.second);
      }

      template<typename F, typename T>
      static auto
      flatMap(F f, T mx){
        auto [y, w] = f(mx.first);
        return pair(y, run(logContext, mappend(w, mx.second)));
      }

      template<typename T>
      static auto
      flatten(T mmx){
        return pair(mmx.first.first,run(logContext, mappend(mmx.first.second, mmx.second)));
      }

      template<typename T>
      static auto
      writer(T mx){ return mx; }

      template<typename W>
      static auto
      tell(W w){
        return pair(unit, w);
      }
      template<typename T>
      static auto
      listen(T aw){
        return pair(aw, aw.second);
      }

      template<typename T>
      static auto
      pass(T mx){
        auto [xf, w] = mx;
        auto [x, f] = xf;
        return pair(x, f(w));
      }

    };
  } // end of anonymous namespace

  TEST(MonadWriter, pure)
  {
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        pure(3)),
      pair(3, Nil{}));
  }

  TEST(MonadWriter, Tell)
  {
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        tell(list(string("This is something I did.")))),
      pair(unit,list(string("This is something I did."))));
  }

  TEST(MonadWriter, Listen){
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        listen(
          beginM(
            tell(list(string("This is the value"))),
            returnM(3)))),
      pair(pair(3, list(string("This is the value"))),
           list(string("This is the value"))));
  }

  TEST(MonadWriter, Pass){
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        pass(beginM(
               tell(list(string("This is the log"))),
               returnM(pair(3, [](auto log){ return run(ListMonoid{}, mappend(log, log)); }))))),
      pair(3,list(string("This is the log"), string("This is the log"))));
  }

  TEST(MonadWriter, Writer){
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        writer(pair(3,list(string("x"), string("y"))))),
      pair(3,list(string("x"), string("y"))));
  }

  TEST(MonadWriter, CensorManual){
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        pass(letF(writer(pair(3, list(string("x"),string("y")))), [](auto x){ return
                pair(x, [](auto logs){ return drop(logs, 1); }); }))),
      pair(3, list(string("y"))));
  }

  TEST(MonadWriter, Censor){
    EXPECT_EQ(
      run(LoggingContext<ListMonoid>{})(
        censor([](auto logs){ return drop(logs, 1); },
               writer(pair(3, list(string("x"),string("y")))))),
      pair(3, list(string("y"))));
  }

} // end of namespace Contextual::Details::Testing
