//
// ... Standard header files
//
#include <type_traits>

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
#include <contextual/details/import.hpp>
#include <contextual/details/Deriving.hpp>
#include <contextual/details/MonadPlus.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    using std::result_of_t;

    using ListProcessing::Dynamic::nil;
    using ListProcessing::Dynamic::list;
    using ListProcessing::Dynamic::ListType;

    constexpr auto pure  = MonadPlus::pure;
    constexpr auto mZero = MonadPlus::mZero;
    constexpr auto mPlus = MonadPlus::mPlus;

    class ProtoListContext{
      static constexpr auto u = [](auto f){ return f(f); };
    public:
      static constexpr auto mZero = []<typename T>(Type<T>){ return nil<T>; };
      static constexpr auto mPlus = [](auto xs, auto ys){ return append(xs, ys); };
      static constexpr auto pure = [](auto x){ return list(x); };
      static constexpr auto flatMap =
        u(curry<3>([](auto rec, auto f, auto xs) -> decltype(f(head(xs))) {
          using R = decltype(f(head(xs)));
          return hasData(xs)
            ? rec(rec)(f, append(f(head(xs)),tail(xs)))
            : R::nil; }));

    }; // end of class ProtoListContext

    class ListContext : public Derive<ProtoListContext, MixinMonadPlus>
    {} constexpr listContext{};

  } // end of anonymous namespace

  TEST(MonadPlus, ListPure){
    EXPECT_EQ(run(listContext, pure(3)), list(3));
  }

  TEST(MonadPlus, ListMPlus){
    EXPECT_EQ(
      run(listContext)(
        mPlus(list(1,2), list(3,4))),
      list(1, 2, 3, 4));
  }

  TEST(MonadPlus, ListMPlusMZero){
    EXPECT_EQ(
      run(listContext)(
        mPlus(list(1, 2), mZero(type<int>))),
      list(1, 2));
  }

  TEST(MonadPlus, ListMPlusMzeroCommutes){
    EXPECT_EQ(
      run(listContext, mPlus(list(1, 2), mZero(type<int>))),
      run(listContext, mPlus(mZero(type<int>), list(1, 2))));
  }

} // end of namespace Contextual::Details::Testing
