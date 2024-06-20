#pragma once

//
// ... Contextual header files
//
#include <contextual/details/Monad.hpp>
#include <contextual/details/Monoid.hpp>
#include <contextual/details/import.hpp>
#include <contextual/details/Unit.hpp>
#include <contextual/details/Derive.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasTell = requires{{Context::tell};};

  template<typename Context>
  concept MissingTell = ! HasTell<Context>;

  template<typename Context>
  concept HasWriter = requires{{Context::writer};};

  template<typename Context>
  concept MissingWriter = ! HasWriter<Context>;

  template<typename Context>
  concept HasListen = requires{{Context::listen};};

  template<typename Context>
  concept MissingListen = ! HasListen<Context>;

  template<typename Context>
  concept HasPass = requires{{Context::pass};};

  template<typename Context>
  concept MissingPass = ! HasPass<Context>;

  template<typename Context>
  concept HasCensor = requires{{Context::censor};};

  template<typename Context>
  concept MissingCensor = ! HasCensor<Context>;

  template<typename Context>
  concept HasMinimalMonadWriter =
    HasMinimalMonad<Context> &&
    (HasTell<Context> || HasWriter<Context>) &&
    HasListen<Context> &&
    HasPass<Context>;

  template<typename Context>
  concept HasPureAndTell =
    HasPure<Context> &&
    HasTell<Context>;

  template<typename Context>
  concept HasFMapAndPass =
    HasFMap<Context> &&
    HasPass<Context>;

  template<typename Context>
  concept HasMonadWriter =
    HasMonadUtility<Context> &&
    HasTell<Context> &&
    HasWriter<Context> &&
    HasListen<Context> &&
    HasPass<Context> &&
    HasCensor<Context>;



  /**
   * @brief Mixin the methods for a writer monad
   */
  class MixinMonadWriter : public Static_curried<MixinMonadWriter, Nat<1>>{

    //  _____    _ _
    // |_   _|__| | |
    //   | |/ -_) | |
    //   |_|\___|_|_|
    class MixinTell : public Static_curried<MixinTell, Nat<1>>{
      template<HasWriter Base>
      class Result : public Base {
        class Tell : public Static_curried<Tell, Nat<1>> {
        public:
          template<typename T>
          static constexpr auto
          call(T&& w){ return Base::writer(pair(unit, std::forward<T>(w))); }
        }; // end of class Tell
      public:
        using Base::Base;
        static constexpr Tell tell{};
      }; // end of class Result

    public:
      template<HasWriter Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasTell<Result<Base>>);
      }
    } static constexpr mixinTell{};

    // __      __   _ _
    // \ \    / / _(_) |_ ___ _ _
    //  \ \/\/ / '_| |  _/ -_) '_|
    //   \_/\_/|_| |_|\__\___|_|
    class MixinWriter : public Static_curried<MixinWriter, Nat<1>>{
      template<HasPureAndTell Base>
      class Result : public Base {
        class Writer : public Static_curried<Writer, Nat<1>>{
        public:
          template<typename T>
          static constexpr auto
          call(T&& aw){
            return Base::flatMap(
              constant(pure(std::get<0>(std::forward<T>(aw)))),
              Base::tell(std::get<1>(std::forward<T>(aw))));
          }
        }; // end of class Writer
      public:
        using Base::Base;
        static constexpr Writer writer{};
      }; // end of class Result

    public:
      template<HasPureAndTell Base>
      static constexpr auto
      call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasWriter<Result<Base>>);
      }
    } static constexpr mixinWriter{};


    //   ___
    //  / __|___ _ _  ___ ___ _ _
    // | (__/ -_) ' \(_-</ _ \ '_|
    //  \___\___|_||_/__/\___/_|
    class MixinCensor : public Static_curried<MixinCensor, Nat<1>>
    {
      template<HasFMapAndPass Base>
      class Result : public Base {
        class Censor : public Static_curried<Censor, Nat<2>>{
        public:
          template<typename F, typename T>
          static constexpr auto
          call(F&& f, T&& mx){
            return Base::pass(
              Base::fMap(
                [f = std::forward<F>(f)]<typename U>(U&& x){ return pair(std::forward<U>(x), std::move(f)); },
                std::forward<T>(mx)));
          }
        };
      public:
        using Base::Base;
        static constexpr Censor censor{};
      };
    public:
      template<HasFMapAndPass Base>
      static constexpr auto
      call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasCensor<Result<Base>>);
      }
    } static constexpr mixinCensor{};

    ////////////////////////////////////////////////////////////////////////

  public:
    template<HasMinimalMonadWriter Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadCore<Base>){
        return call(mixinMonad(type<Base>));

      } else if constexpr (MissingMonadUtility<Base>){
        return call(mixinMonadUtility(type<Base>));

      } else if constexpr (MissingTell<Base>){
        return call(mixinTell(type<Base>));

      } else if constexpr (MissingWriter<Base>){
        return call(mixinWriter(type<Base>));

      } else if constexpr (MissingCensor<Base>){
        return call(mixinCensor(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadWriter<Base>);
      }
    }
  } constexpr mixinMonadWriter{};

  class ProtoMonadWriter : public Monad {
    class Tell : public Static_curried<Tell, Nat<1>>{
      static constexpr auto askTell =
        asksC2([]<typename Context, typename W>(Context, W&& w) {
            return Context::tell(std::forward<W>(w));
          });
    public:
      template<typename W>
      static constexpr auto
      call(W&& w){
        // clang-format off
        return
          letC(askTell, [=](auto tell) { return
                returnC(tell(w)); });
        // clang-format on
      }
    }; // end of class Tell

    class Writer : public Static_curried<Writer, Nat<1>>{
      static constexpr auto askWriter =
        asksC2([]<typename Context, typename Pair>(Context, Pair&& aw) {
            return Context::writer(std::forward<Pair>(aw));
          });
    public:
      template<typename AW>
      static constexpr auto
      call(AW const& aw){
        // clang-format off
        return
          letC(askWriter, [=](auto writer){ return
                returnC( writer(aw)); });
        // clang-format on
      }
    };  // end of class Writer

    class Listen : public Static_curried<Listen, Nat<1>>{
      static constexpr auto askListen =
        asksC2([]<typename Context, typename T>(Context, T&& aw) {
            return Context::listen(std::forward<T>(aw));
          });
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmx){
        // clang-format off
        return
          letC(askListen, [=](auto listen){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(listen(mx)); }); });
        // clang-format on
      }
    }; // end of class Listen

    class Pass : public Static_curried<Pass, Nat<1>>{
      static constexpr auto askPass =
        asksC2([]<typename Context, typename Pair>(Context, Pair&& maf) {
            return Context::pass(std::forward<Pair>(maf));
          });
    public:
      template<typename T>
      static constexpr auto
      call(T&& cmx){
        return
          letC(askPass, [=](auto pass){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(pass(mx)); }); });
      }
    }; // end of class Pass
  public:
    static constexpr Tell tell{};
    static constexpr Writer writer{};
    static constexpr Listen listen{};
    static constexpr Pass pass{};

  }; // end of class ProtMonadWriter

  class MonadWriter : public Derive<ProtoMonadWriter, MixinMonadWriter>
  {};




} // end of namespace Contextual::Details
