#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Monad.hpp>

namespace Contextual::Details
{

  template<typename Context>
  concept HasAsk = requires{{Context::ask};};

  template<typename Context>
  concept MissingAsk = ! HasAsk<Context>;

  template<typename Context>
  concept HasLocal = requires{{Context::local};};

  template<typename Context>
  concept MissingLocal = ! HasLocal<Context>;

  template<typename Context>
  concept HasAsks = requires{{Context::asks};};

  template<typename Context>
  concept MissingAsks = ! HasAsks<Context>;

  template<typename Context>
  concept HasMinimalMonadReader =
    (HasAsk<Context> || HasAsks<Context>) &&
    HasLocal<Context>;

  template<typename Context>
  concept MissingMinimalMonadReader = ! HasMinimalMonadReader<Context>;


  template<typename Context>
  concept HasMonadReaderCore =
    HasMonadCore<Context> &&
    HasAsk<Context> &&
    HasAsks<Context> &&
    HasLocal<Context>;


  template<typename Context>
  concept MissingMonadReaderCore = ! HasMonadReaderCore<Context>;

  template<typename Context>
  concept HasMonadReaderUtility =
    HasMonadUtility<Context> &&
    HasMonadReaderCore<Context>;


  template<typename Context>
  concept MissingMonadReaderUtility = ! HasMonadReaderUtility<Context>;


  template<typename Concept>
  concept HasMonadReader =
    HasMonadReaderCore<Concept> &&
    HasMonadReaderUtility<Concept>;

  template<typename Context>
  concept MissingMonadReader = ! HasMonadReader<Context>;

  template<typename Context>
  concept HasFMapAndAsk = HasFMap<Context> && HasAsk<Context>;





  class MixinMonadReaderCore : public Static_curried<MixinMonadReaderCore, Nat<1>>{
    //    _       _
    //   /_\   __| |__
    //  / _ \ (_-< / /
    // /_/ \_\/__/_\_\.
    class MixinAsk : public Static_curried<MixinAsk, Nat<1>>
    {
      template<HasAsks Base>
      class Result : public Base
      {
      public:
        using Base::Base;
        static constexpr auto ask = Base::asks(identity);
      };

    public:
      template<HasAsks Base>
      static constexpr auto call(Type<Base>)
      {
        return type<Result<Base>>;
        static_assert(HasAsk<Result<Base>>);
      }
    } static constexpr mixinAsk{};

    //    _       _
    //   /_\   __| |__ ___
    //  / _ \ (_-< / /(_-<
    // /_/ \_\/__/_\_\/__/
    class MixinAsks : public Static_curried<MixinAsks, Nat<1>>
    {
      template<HasFMapAndAsk Base>
      class Result : public Base
      {
        class Asks : public Static_curried<Asks, Nat<1>>
        {
        public:
          template<typename F>
          static constexpr auto
          call(F&& f){ return Base::fMap(forward<F>(f), Base::ask); }
        };

      public:
        using Base::Base;
        static constexpr Asks asks{};
      };

    public:
      template<HasFMapAndAsk Base>

      static constexpr auto
      call(Type<Base>){
        return type<Result<Base>>;
        static_assert(HasAsks<Result<Base>>);
      }
    } static constexpr mixinAsks{};



  public:
    template<HasMinimalMonadReader Base>
    static constexpr auto
    call(Type<Base>){
      if constexpr (MissingMonadCore<Base>){
        return call(mixinMonad(type<Base>));

      } else if constexpr (MissingAsk<Base>){
        return call(mixinAsk(type<Base>));

      } else if constexpr (MissingAsks<Base>){
        return call(mixinAsks(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadReaderCore<Base>);

      }
    }
  } constexpr mixinMonadReaderCore{};


  class MixinMonadReaderUtility : public Static_curried<MixinMonadReaderUtility, Nat<1>>{
  public:

    template<HasMonadReaderCore Base>
    static constexpr auto
    call(Type<Base>){

      if constexpr (MissingMonadUtility<Base>){
        return call(mixinMonadUtility(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadReaderUtility<Base>);

      }
    } // end of function call
  } constexpr mixinMonadReaderUtility{};

  class MixinMonadReader : public Static_curried<MixinMonadReader, Nat<1>>{
  public:
    template<HasMinimalMonadReader Base>
    static auto
    call(Type<Base>){

      if constexpr (MissingMonadReaderCore<Base>){
        return call(mixinMonadReaderCore(type<Base>));

      } else if constexpr (MissingMonadReaderUtility<Base>){
        return call(mixinMonadReaderUtility(type<Base>));

      } else {
        return type<Base>;
        static_assert(HasMonadReader<Base>);
      }
    }
  } constexpr mixinMonadReader{};


  class ProtoMonadReader : public Monad
  {
    class Local : public Static_curried<Local, Nat<2>>{
     static constexpr auto askLocal =
       asksC3([]<typename Context, typename F, typename T>
              (Context, F&& f, T&& mx){
                return Context::local(forward<F>(f), forward<T>(mx));
              });

    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& f, T&& cmx){
        // clang-format off
        return
          letC(askLocal, [=](auto local){ return
          letC(injest(cmx), [=](auto mx){ return
                returnC(local(f, mx)); }); });
      }
    };
  public:
    constexpr static Local local{};
    constexpr static auto ask = asksC([]<typename Context>(Context){ return Context::ask; });
  };

  class MonadReader : public Derive<ProtoMonadReader, MixinMonadReader>
  {};
} // end of namespace Contextual::Details
