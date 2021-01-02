//
// ... Standard header files
//
#include <tuple>
#include <string>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>
#include <list_processing/dynamic.hpp>

//
// ... Contextual header files
//
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Monoid.hpp>
#include <contextual/details/MonadRWS.hpp>
#include <contextual/details/Unit.hpp>

namespace Contextual::Details::Testing
{

  namespace // anonymous
  {
    using std::make_tuple;
    using std::string;

    using TypeUtility::type;

    using FunctionUtility::identity;

    using ListProcessing::Dynamic::list;
    using ListProcessing::Dynamic::Nil;
    using ListProcessing::Dynamic::ListType;



    constexpr auto mEmpty  = Monoid::mEmpty;
    constexpr auto mAppend = Monoid::mAppend;

    constexpr auto pure    = MonadRWS::pure;
    constexpr auto returnM = MonadRWS::returnM;
    constexpr auto fMap    = MonadRWS::fMap;
    constexpr auto letF    = MonadRWS::letF;
    constexpr auto fApply  = MonadRWS::fApply;
    constexpr auto flatMap = MonadRWS::flatMap;
    constexpr auto letM    = MonadRWS::letM;
    constexpr auto beginM  = MonadRWS::beginM;
    constexpr auto flatten = MonadRWS::flatten;
    constexpr auto ask     = MonadRWS::ask;
    constexpr auto asks    = MonadRWS::asks;
    constexpr auto local   = MonadRWS::local;
    constexpr auto writer  = MonadRWS::writer;
    constexpr auto tell    = MonadRWS::tell;
    constexpr auto listen  = MonadRWS::listen;
    constexpr auto pass    = MonadRWS::pass;
    constexpr auto censor  = MonadRWS::censor;
    constexpr auto get     = MonadRWS::get;
    constexpr auto put     = MonadRWS::put;
    constexpr auto select  = MonadRWS::select;
    constexpr auto modify  = MonadRWS::modify;


    class ListContext
    {
    public:

      static constexpr Nil mEmpty{};

      template<typename List1, typename List2>
      static auto
      op(List1 xs, List2 ys){ return append(xs, ys); }

      template<typename List>
      static auto
      op(List xs, Nil){ return xs; }

      template<typename List>
      static auto
      op(Nil, List ys){ return ys; }

    }; // ListContext


    template<typename T>
    std::ostream&
    operator <<(std::ostream& os, ListType<T> xs){
      os << "(";
      if(hasData(xs)){
        os << head(xs);
        doList(
          tail(xs),
          [&](auto x){ os << ", " << x; });
      }
      os << ")";
      return os;
    }


    template<typename LogContext>
    class RWSContext
      : public Deriving<
      RWSContext<LogContext>,
      MonadRWS::MonadicFlatten
      >
    {
    public:
      static constexpr LogContext logContext{};

      template<typename F, typename T>
      static auto
      fMap(F f, T mx){
        return [=](auto es){
          auto [x, w, s] = run(es, mx);
          return make_tuple(f(x), w, s);
        };
      }

      template<typename T>
      static auto
      pure(T x){
        return [=](auto es){
          return make_tuple(x, run(logContext, mEmpty), std::get<1>(es));
        };
      }

      template<typename F, typename T>
      static auto
      fApply(F mf, T mx){
        return [=](auto es){
          auto [f, wf, sf] = run(es, mf);
          auto [x, wx, sx] = run(make_tuple(std::get<0>(es), sf), mx);
          return make_tuple(f(x), run(logContext, mAppend(wf, wx)), sx);
        };
      }

      template<typename F, typename T>
      static auto
      flatMap(F f, T mx){
        return [=](auto es){
          auto [x, log, state] = run(es, mx);
          return run(make_tuple(std::get<0>(es), state), f(x));
        };
      }

      static constexpr auto ask = [](auto es){
        return make_tuple(std::get<0>(es), run(logContext, mEmpty), std::get<1>(es));
      };


      template<typename F, typename T>
      static constexpr auto
      local(F f, T mx){
        return [=](auto es){
          auto [env, state] = es;
          return run(make_tuple( f(env), state), mx); };
      }

      template<typename T>
      static auto
      writer(T xw){
        return [=](auto es){
          return make_tuple( std::get<0>(xw), std::get<1>(xw), std::get<1>(es)); };
      }

      template<typename W>
      static auto
      tell(W w){
        return [=](auto es){ return make_tuple(unit, run(logContext, injest(w)), std::get<1>(es)); };
      }

      template<typename T>
      static auto
      listen(T mx){
        return [=](auto es){
          auto [x, log, state] = run(es, mx);
          return make_tuple(make_tuple(x, log), log, state);
        };
      }

      template<typename T>
      static auto
      pass(T mx){
        return [=](auto es){
          auto [xf, w, s] = run(es, mx);
          auto [x, f] = xf;
          return make_tuple(x, f(w), s);
        };
      }

      static constexpr auto get = [](auto es){ return run( es, pure(std::get<1>(es))); };

      template<typename S>
      static constexpr auto
      put(S s){
        return [=](auto){
          return make_tuple(unit, run(logContext, mEmpty), s);
        };
      }
    };

  } // end of anonymous namespace

  TEST(MonadRWS, Pure){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          pure('x'))),
      make_tuple('x', Nil{}, 's'));
  }

  TEST(MonadRWS, Return){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          returnM('x'))),
      make_tuple('x', Nil{}, 's'));
  }

  TEST(MonadRWS, FMap){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          fMap([](auto x){ return x+x; }, returnM(3)))),
      make_tuple(6, Nil{}, 's'));
  }

  TEST(MonadRWS, FApply){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          fApply(writer(make_tuple([](auto x){ return x+x; }, list(string("f")))),
                 writer(make_tuple(3, list(string("x"))))))),
      make_tuple(6, list(string("f"), string("x")), 's'));
  }

  TEST(MonadRWS, FlatMap){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          flatMap(put, get))),
      make_tuple(unit, Nil{}, 's'));
  }

  TEST(MonadRWS, LetM){
    EXPECT_EQ(
      run(make_tuple([](auto x){ return x+1; }, 0))(
        run(RWSContext<ListContext>{})(
          letM(get, [=](auto x){ return
          letM(ask, [=](auto f){ return
                beginM(
                  put(f(x)),
                  returnM(x)); });}))),
      make_tuple(0, Nil{}, 1));
  }

  TEST(MonadRWS, Flatten){ }

  TEST(MonadRWS, Ask){
    EXPECT_EQ(
      run(make_tuple('e','s'))(
        run(RWSContext<ListContext>{})(
          ask)),
      make_tuple('e', Nil{}, 's'));
  }

  TEST(MonadRWS, Asks){
    EXPECT_EQ(
      run(make_tuple('e','s'))(
        run(RWSContext<ListContext>{})(
          asks([](char e){ return string(1,e); }))),
      make_tuple(string("e"), Nil{}, 's'));
  }

  TEST(MonadRWS, Local){ }

  TEST(MonadRWS, Tell){
    EXPECT_EQ(
      run(make_tuple('e','s'))(
        run(RWSContext<ListContext>{})(
          tell(list(string("Something happened!"))))),
      make_tuple(unit, list(string("Something happened!")), 's'));
  }
  TEST(MonadRWS, Writer){
    EXPECT_EQ(
      run(make_tuple('e','s'))(
        run(RWSContext<ListContext>{})(
          writer(make_tuple('x', list(string("x")))))),
      make_tuple('x', list(string("x")), 's'));
  }

  TEST(MonadRWS, Listen)
  {
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          listen(writer(make_tuple('x', list('x')))))),
      make_tuple(make_tuple('x',list('x')), list('x'), 's'));
  }

  TEST(MonadRWS, Pass){
    EXPECT_EQ(
      run(make_tuple('e','s'))(
        run(RWSContext<ListContext>{})(
          pass(pure(make_tuple(3, [](auto logs){ return cons(3, logs); }))))),
      make_tuple(3, list(3), 's'));
  }

  TEST(MonadRWS, Censor){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          censor([](auto logs){ return cons(3, logs); },
                 returnM(3)))),
      make_tuple(3, list(3), 's'));
  }

  TEST(MonadRWS, Get){
    EXPECT_EQ(
      run(make_tuple('e', 's'))(
        run(RWSContext<ListContext>{})(
          get)),
      make_tuple('s', Nil{}, 's'));
  }
  TEST(MonadRWS, Put){
    EXPECT_EQ(
      run(make_tuple('e', string("original state")))(
        run(RWSContext<ListContext>())(
          put(string("new state")))),
      make_tuple(unit, Nil(), string("new state")));
  }

  TEST(MonadRWS, Modify){
    EXPECT_EQ(
      run(make_tuple('e', 0))(
        run(RWSContext<ListContext>{})(
          modify([](auto x){ return x+1; }))),
      make_tuple(Unit{}, Nil{}, 1));
  }


} // end of namespace Contextual::Details::Testing
