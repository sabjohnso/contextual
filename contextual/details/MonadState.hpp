#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Unit.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasState = requires{{Context::state};};

  template<typename Context>
  concept MissingState = ! HasState<Context>;

  template<typename Context>
  concept HasPut = requires{{Context::put};};

  template<typename Context>
  concept MissingPut = ! HasPut<Context>;

  template<typename Context>
  concept HasGet = requires{{Context::get};};

  template<typename Context>
  concept MissingGet = ! HasGet<Context>;

  template<typename Context>
  concept HasModify = requires{{Context::modify};};

  template<typename Context>
  concept MissingModify = ! HasModify<Context>;

  template<typename Context>
  concept HasSelect = requires{{Context::select};};

  template<typename Context>
  concept MissingSelect = ! HasSelect<Context>;


  template<typename Context>
  concept HasMinimalMonadState =
    HasMinimalMonad<Context> &&
    (HasState<Context> ||
     (HasPut<Context> || HasModify<Context>) &&
     (HasGet<Context> || HasSelect<Context>));

  template<typename Context>
  concept MissingMinimalMonadState = ! HasMinimalMonadState<Context>;

  template<typename Context>
  concept HasMonadStateCore =
    HasMonadCore<Context> &&
    HasState<Context> &&
    HasGet<Context> &&
    HasSelect<Context> &&
    HasPut<Context> &&
    HasModify<Context>;

  template<typename Context>
  concept HasPutAndGet =
    HasPut<Context> &&
    HasGet<Context>;

  template<typename Context>
  concept HasFMapAndGet =
    HasFMap<Context> &&
    HasGet<Context>;

  template<typename Context>
  concept HasFlatMapPutAndSelect =
    HasFlatMap<Context> &&
    HasPut<Context> &&
    HasSelect<Context>;

  class MixinMonadState : public Static_curried<MixinMonadState, Nat<1>>{
    //  ___ _        _
    // / __| |_ __ _| |_ ___
    // \__ \  _/ _` |  _/ -_)
    // |___/\__\__,_|\__\___|
    class MixinState : public Static_curried<MixinState, Nat<1>>{
      template<HasPutAndGet Base>
      class Result : public Base{
        class State : public Static_curried<State, Nat<1>>{

          template<typename T, typename S>
          static constexpr auto
          inject(T&& x, S&& s){
            return Base::flatMap([x = forward<T>(x)](auto){ return Base::pure(x); },
                                 Base::put(forward<S>(s)));
          }
          template<typename FS>
          static constexpr auto
          expand(FS&& fs){
            return inject(std::get<0>(forward<FS>(fs)),
                          std::get<1>(forward<FS>(fs)));
          }

        public:
          template<typename F>
          static constexpr auto
          call(F&& f){
            return Base::flatMap(
              [f = forward<F>(f)]<typename S>(S&& s){ return expand(f(forward<S>(s))); },
              Base::get);
          }
        };

      public:
        using Base::Base;
        static constexpr State state{};
      };

    public:
      template<HasPutAndGet Base>
      static constexpr auto
      call(Type<Base>){ return type<Result<Base>>; }
    } static constexpr mixinState{};

    //   ___     _
    //  / __|___| |_
    // | (_ / -_)  _|
    //  \___\___|\__|
    class MixinGet : public Static_curried<MixinGet, Nat<1>>{
      template<HasSelect Base>
      class Result : public Base{
      public:
        using Base::Base;
        static constexpr auto get = Base::select(identity);
      };

    public:
      template<HasSelect Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasGet<Result<Base>>);
      }
    } static constexpr mixinGet{};


    class MixinGetFromState : public Static_curried<MixinGetFromState, Nat<1>>{
      template<HasState Base>
      class Result : public Base{
      public:
        using Base::Base;
        static constexpr auto get = Base::state([](auto s){ return pair(s,s); });
      };
    public:
      template<HasState Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasGet<Result<Base>>);
      }
    } static constexpr mixinGetFromState{};

    //  ___      _        _
    // / __| ___| |___ __| |_
    // \__ \/ -_) / -_) _|  _|
    // |___/\___|_\___\__|\__|
    class MixinSelect : public Static_curried<MixinSelect, Nat<1>>{
      template<HasFMapAndGet Base>
      class Result : public Base {
        class Select : public Static_curried<Select, Nat<1>>{
        public:
          template<typename F>
          static constexpr auto
          call(F&& f){ return Base::fMap(forward<F>(f), Base::get); }
        };
      public:
        using Base::Base;
        static constexpr Select select{};
      };
    public:
      template<HasFMapAndGet Base>
      static constexpr auto
      call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasSelect<Result<Base>>);
      }
    } static constexpr mixinSelect{};

    //  ___      _
    // | _ \_  _| |_
    // |  _/ || |  _|
    // |_|  \_,_|\__|
    class MixinPut : public Static_curried<MixinPut, Nat<1>>
    {
      template<HasModify Base>
      class Result : public Base {
        class Put : public Static_curried<Put, Nat<1>>{
        public:
          template<typename S>
          static constexpr auto
          call(S&& s){ return Base::modify(constant(forward<S>(s))); }
        };

      public:
        using Base::Base;
        static constexpr Put put{};
      };

    public:
      template<HasModify Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasPut<Result<Base>>);
      }
    } static constexpr mixinPut{};

    class MixinPutFromState : public Static_curried<MixinPutFromState, Nat<1>>{
      template<HasState Base>
      class Result : public Base{
        class Put : public Static_curried<Put, Nat<1>>{
        public:
          template<typename S>
          static constexpr auto
          call(S&& s){
            return Base::state([s = forward<S>(s)](auto){
              return pair(unit, s); });
          }
        };
      public:
        using Base::Base;
        static constexpr Put put{};
      };

    public:
      template<HasState Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasPut<Result<Base>>);
      }
    } static constexpr mixinPutFromState{};

    //  __  __         _ _  __
    // |  \/  |___  __| (_)/ _|_  _
    // | |\/| / _ \/ _` | |  _| || |
    // |_|  |_\___/\__,_|_|_|  \_, |
    //                         |__/
    class MixinModify : public Static_curried<MixinModify, Nat<1>>
    {
      template<HasFlatMapPutAndSelect Base>
      class Result : public Base {
        class Modify : public Static_curried<Modify, Nat<1>>{
        public:
          template<typename F>
          static constexpr auto
          call(F&& f){
            return Base::flatMap(Base::put, Base::select(forward<F>(f)));
          }
        };
      public:
        using Base::Base;
        static constexpr Modify modify{};
      };

    public:
      template<HasFlatMapPutAndSelect Base>
      static constexpr auto call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasModify<Result<Base>>);
      }
    } static constexpr mixinModify{};


    ////////////////////////////////////////////////////////////////////////

  public:
    template<HasMinimalMonadState Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadCore<Base>){
        return call(mixinMonadCore(type<Base>));

      } else if constexpr (MissingState<Base>){

        if constexpr (MissingGet<Base>){
          return call(mixinGet(type<Base>));

        } else if constexpr (MissingSelect<Base>){
          return call(mixinSelect(type<Base>));

        } else if constexpr (MissingPut<Base>){
          return call(mixinPut(type<Base>));

        } else if constexpr (MissingModify<Base>){
          return call(mixinModify(type<Base>));

        } else {
          return call(mixinState(type<Base>));
        }

      } else {
        if constexpr (MissingGet<Base>){
          return call(mixinGetFromState(type<Base>));

        } else if constexpr (MissingSelect<Base>){
          return call(mixinSelect(type<Base>));

        } else if constexpr (MissingPut<Base>){
          return call(mixinPutFromState(type<Base>));

        } else if constexpr (MissingModify<Base>){
          return call(mixinModify(type<Base>));

        } else {
          return type<Base>;
          static_assert(HasMonadStateCore<Base>);
        }
      }
    }
  } constexpr mixinMonadState{};


  class MixinMonadStateUtility : public Static_curried<MixinMonadStateUtility,  Nat<1>>{
  public:
    template<HasMonadStateCore Base>
    static constexpr auto
    call(Type<Base>){ return type<Base>; }
  } constexpr mixinMonadStateUtility{};


  class ProtoMonadState : public Monad {
    class Put : public Static_curried<Put,Nat<1>> {
      static constexpr auto askPut =
        asksC2([]<typename Context, typename S>
               (Context, S&& s){ return Context::put(forward<S>(s)); });
    public:
      template<typename S>
      static constexpr auto
      call(S&& s){
        return
          letC(askPut, [=](auto put){ return
                returnC(put(s)); });
      }
    };
  public:
    static constexpr Put put{};
    static constexpr auto get = asksC([]<typename Context>(Context){ return Context::get; });
  };
  static_assert(HasMinimalMonadState<ProtoMonadState>);

  class MonadState : public Derive<ProtoMonadState, MixinMonadState, MixinMonadStateUtility>
  {};


} // end of namespace Contextual::Details
