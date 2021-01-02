//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Derive.hpp>

namespace Contextual::Details::Testing
{
  namespace // anonymous
  {
    template<typename T>
    concept HasAdd = requires{{T::add};};

    template<typename T>
    concept MissingAdd = ! HasAdd<T>;

    template<typename T>
    concept HasMul = requires{{T::mul};};

    template<typename T>
    concept MissingMul = ! HasMul<T>;

    template<typename T>
    concept HasFMA = requires{{T::fma};};

    template<typename T>
    concept MissingFMA = ! HasFMA<T>;

    template<typename T>
    concept MinimalProto = HasAdd<T> && HasMul<T>;

    class Proto
    {
    public:
      static constexpr auto add = [](auto x, auto y){ return x+y; };
      static constexpr auto mul = [](auto x, auto y){ return x*y; };
    };

    template<typename T>
    concept Arith = requires
    {
      { Proto::add };
      { Proto::mul };
    };

    class FMAMixin : public Static_callable<FMAMixin>
    {
      template<MinimalProto Base>
      class Result : public Base {
      public:
        using Base::Base;
        static constexpr auto fma = [](auto x, auto y, auto z){ return Base::add(Base::mul(x, y), z); };
      };

    public:

      template<MinimalProto Base>
      static constexpr auto
      call(Type<Base>){
        return type<Result<Base>>;
      }
    };

    class Full : public Derive<Proto, FMAMixin>
    {};


  } // end of namespace // anonymous


  TEST(Derive, SimpleMixin)
  {
    EXPECT_EQ(Full::add(2, 3), 5);
    EXPECT_EQ(Full::mul(2, 3), 6);
    EXPECT_EQ(Full::fma(2, 3, 4), 10);
  }
} // end of namespace Contextual::Details::Testing
