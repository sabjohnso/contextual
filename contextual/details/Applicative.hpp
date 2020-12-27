#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Injest.hpp>
#include <contextual/details/Functor.hpp>

namespace Contextual::Details
{
  class Applicative : public Functor
  {
    static constexpr auto askPure =
      asksC2(
        []<typename Context, typename T>
        (Context, T&& x){ return Context::pure(forward<T>(x)); });

    static constexpr auto askFApply =
      asksC3(
        []<typename Context, typename F, typename T>
        (Context, F&& mf, T&& mx){ return Context::fApply(mf, mx); });

    class Pure : public Static_curried<Pure, Nat<1>>{
    public:
      template<typename T>
      static constexpr auto
      call(T&& x){
        return
          letC(askPure, [=](auto pure){ return
                returnC(pure(x)); });
      }
    };

    class FApply : public Static_curried<FApply, Nat<2>>{
    public:
      template<typename F, typename T>
      static constexpr auto
      call(F&& cmf, T&& cmx){
        return
          letC(askFApply,  [=](auto fApply){ return
          letC(injest(cmf),[&](auto mf){ return
          letC(injest(cmx), [&](auto mx){ return
                returnC(fApply(mf, mx)); }); }); });
      }
    };

  public:
    template<typename Context>
    class ApplicativeFMap
    {
    public:
      template<typename F, typename T>
      static constexpr auto
      fMap(F&& f, T&& mx){
        return Context::fApply(Context::pure(forward<F>(f)), forward<T>(mx));
      }
    };


    static constexpr Pure pure{};
    static constexpr FApply fApply{};

    static constexpr auto fApply2 =
      curry<3>([]<typename F, typename A, typename B>
               (F&& mf, A&& ma, B&& mb){
                 return fApply(fApply(forward<F>(mf), forward<A>(ma)), forward<B>(mb));
               });
    static constexpr auto fApply3 =
      curry<4>([]<typename F, typename A, typename B, typename C>
               (F&& mf, A&& ma, B&& mb, C&& mc){
                 return fApply(fApply2(forward<F>(mf), forward<A>(ma), forward<B>(mb)),
                               forward<C>(mc));
               });


    static constexpr auto fApply4 =
      curry<5>([]<typename F, typename A, typename B, typename C, typename D>
               (F&& mf, A&& ma, B&& mb, C&& mc, D&& md){
                 return fApply(
                   fApply3(
                     forward<F>(mf),
                     forward<A>(ma),
                     forward<B>(mb),
                     forward<C>(mc)),
                   forward<D>(md));});

    static constexpr auto fApply5 =
      curry<6>([]<typename F, typename A, typename B, typename C, typename D, typename E>
               (F&& mf, A&& ma, B&& mb, C&& mc, D&& md, E&& me){
                 return fApply(
                   fApply4(
                     forward<F>(mf),
                     forward<A>(ma),
                     forward<B>(mb),
                     forward<C>(mc),
                     forward<D>(md)),
                   forward<E>(me));});


    static constexpr auto liftM2 =
      curry<3>([]<typename F, typename A, typename B>
               (F&& f, A&& ma, B&& mb){
                 return fApply2(
                   pure(forward<F>(f)),
                   forward<A>(ma),
                   forward<B>(mb)); });


    static constexpr auto liftM3 =
      curry<4>([]<typename F, typename A, typename B, typename C>
               (F&& f, A&& ma, B&& mb, C&& mc){
                 return fApply3(
                   pure(forward<F>(f)),
                   forward<A>(ma),
                   forward<B>(mb),
                   forward<C>(mc)); });

    static constexpr auto liftM4 =
      curry<5>([]<typename F, typename A, typename B, typename C, typename D>
               (F&& f, A&& ma, B&& mb, C&& mc, D&& md){
                 return fApply4(
                   pure(forward<F>(f)),
                   forward<A>(ma),
                   forward<B>(mb),
                   forward<C>(mc),
                   forward<D>(md)); });

    static constexpr auto liftM5 =
      curry<5>([]<typename F, typename A, typename B, typename C, typename D, typename E>
               (F&& f, A&& ma, B&& mb, C&& mc, D&& md, D&& me){
                 return fApply5(
                   pure(forward<F>(f)),
                   forward<A>(ma),
                   forward<B>(mb),
                   forward<C>(mc),
                   forward<D>(md),
                   forward<E>(me)); });



  }; // end of class Applicative

} // end of namespace Contextual::Details
