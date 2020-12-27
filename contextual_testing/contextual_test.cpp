//
// ... Standard header files
//
#include <string>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... Contextual header files
//
#include <contextual/details/Contextual.hpp>

using Contextual::Details::run;
using Contextual::Details::returnC;
using Contextual::Details::flatMapC;
using Contextual::Details::askC;
using Contextual::Details::asksC;

using namespace std::literals::string_literals;

namespace Contextual::Testing
{
  TEST(Contextual, Return){
    EXPECT_EQ(run("e"s, returnC("x"s)), "x"s);
  }

  TEST(Contextual, ReturnCurried){
    EXPECT_EQ(run("e"s)(returnC("x"s)), "x"s);
  }

  TEST(Contextual, FlatMap){
    EXPECT_EQ(run("e"s,flatMapC(returnC, returnC("x"s))), "x"s);
  }

  TEST(Contextual, AskC){
    EXPECT_EQ(run("e"s, askC), "e"s);
  }

  TEST(Contextual, AsksC){
    EXPECT_EQ(run("e"s, asksC([](auto x){ return x + x; })), "ee"s);
  }
} // end of namespace Contextual::Testing
