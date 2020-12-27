//
// ... Standard header files
//
#include <utility>
#include <functional>
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
#include <contextual/details/Run.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/Unit.hpp>
#include <contextual/details/MonadState.hpp>

namespace Contextual::Details::Testing
{

  namespace // anonymous
  {
    using std::string;
    using std::function;
    using std::pair;

    using ListProcessing::Dynamic::ListType;
    using ListProcessing::Dynamic::list;

    constexpr auto pure    = MonadState::pure;
    constexpr auto returnM = MonadState::returnM;
    constexpr auto get     = MonadState::get;
    constexpr auto put     = MonadState::put;
    constexpr auto fMap    = MonadState::fMap;
    constexpr auto letF    = MonadState::letF;
    constexpr auto flatMap = MonadState::flatMap;
    constexpr auto letM    = MonadState::letM;
    constexpr auto beginM  = MonadState::beginM;

    template<typename Context>
    using MonadicFApply = MonadState::MonadicFApply<Context>;

    template<typename Context>
    using MonadicFApply = MonadState::MonadicFApply<Context>;

    template<typename Context>
    using MonadicFlatten = MonadState::MonadicFlatten<Context>;

  } // end of namespace // anonymous



  template<typename S>
  class StateContext
    : public Deriving<
    StateContext<S>,
    MonadicFApply,
    MonadicFlatten
    >
  {
  public:

    template<typename T>
    using Stateful = function<pair<T,S>(S)>;

    template<typename T>
    static Stateful<T>
    pure(T x){ return [=](S s){ return pair(x, s); }; }

    template<typename F, typename T, typename R = result_of_t<F(T)>>
    static Stateful<R>
    fMap(F f, Stateful<T> mx){
      return [=](S s){
        auto [x, snew] = mx(s);
        return pair(f(x), snew);
      };
    }

    template<typename F, typename T, typename R = result_of_t<F(T)>>
    static auto
    flatMap(F f, Stateful<T> mx){
      return [=](S s){
        auto [x, snew] = run(s, mx);
        return run(snew, f(x));
      };
    }

    static Stateful<Unit>
    put(S s){
      return [=](S){ return pair(unit, s); };
    }

    static Stateful<S> get;
  };

  template<typename S>
  typename StateContext<S>::Stateful<S> StateContext<S>::get([](S s){ return pair(s,s); });

  template<typename S>
  StateContext<S> stateContext{};


  TEST(MonadState, Pure){
    EXPECT_EQ(run('s', run(StateContext<char>{},pure(4))),
              pair(4, 's'));
  }

  TEST(MonadState, Return){
    EXPECT_EQ(run('s', run(StateContext<char>{},returnM('x'))),
              pair('x', 's'));
  }

  TEST(MonadState, FMap){
    EXPECT_EQ(
      run('s', run(StateContext<char>{})(
            fMap([](string str){ return str+str; },
                 returnM(string("x"))))),
      pair(string("xx"), 's'));

  }

  TEST(MonadState, LetF){
    EXPECT_EQ(
      run('s', run(StateContext<char>{})(
            letF(pure(string("x")), [](auto x){
              return x+x; }))),
      pair(string("xx"), 's'));
  }

  TEST(MonadState, FlatMap){
    EXPECT_EQ(
      run('s', run(StateContext<char>{})(
            flatMap([](auto x){ return pure(x+x); },
                    pure(string("x"))))),
      pair(string("xx"),'s'));
  }

  TEST(MonadState, LetM){
    EXPECT_EQ(
      run('s', run(StateContext<char>{})(
            letM(pure(string("x")), [](auto x){ return
                  returnM(x+x); }))),
      pair(string("xx"), 's'));
  }

  TEST(MonadState, Get){
    EXPECT_EQ(
      run('s', run(StateContext<char>{}, get)),
      pair('s','s'));
  }

  TEST(MonadState, Put){
    EXPECT_EQ(
      run('s', run(StateContext<char>{}, put('t'))),
      pair(unit, 't'));
  }

  TEST(MonadState, PostIncrement){
    EXPECT_EQ(
      run(0)(
        run(stateContext<int>)(
          letM(get, [](auto x){ return
                beginM(
                  put(x+1),
                  returnM(x)); }))),
      pair(0, 1));
  }



   class Game
   {
   public:

     using Input = int;
     using InputList = ListType<Input>;
     using State = pair<bool,int>;

     using Context = StateContext<State>;

     template<typename T>
     using Stateful = typename Context::template Stateful<T>;

     using Recur = Contextual<function<Stateful<State>(Context)>>;

     static bool
     status(State s){ return s.first; };

     static int
     score(State s){ return s.second; }


     static State
     update(Input input, State state){
       return input == 0
         ? State(! status(state), score(state))
         : (status(state)
            ? State( true, score(state) + input)
            : state);
     }

     static Recur
     game(InputList inputs){
       return hasData(inputs)
         ? Recur(
           letM(get, [=](State s){ return
                 beginM(
                   put(update(head(inputs), s)),
                   game(tail(inputs))); }))
         : Recur(get);
     }
   };

  TEST(MonadState, Game){
    EXPECT_EQ(
      run(pair(false, 0))(
        run(Game::Context{})(
          Game::game(list(0, 1, -1, 1, 1, 0)))),
      pair(pair(false, 2),pair(false,2)));
  }

} // end of namespace Contextual::Details::Testing
