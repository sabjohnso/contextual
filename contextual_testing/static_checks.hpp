#pragma once

//
// ... Testing header files
//
#include <gtest/gtest.h>


#define STATIC_EXPECT_EQ(x, y)                    \
  do{                                             \
    static_assert((x) == (y));                    \
    ASSERT_EQ((x),(y));                           \
  } while(false)

#define STATIC_EXPECT_TRUE(...)                 \
  do{                                           \
    static_assert(__VA_ARGS__);                 \
    ASSERT_TRUE(__VA_ARGS__);                   \
  } while(false)

#define STATIC_EXPECT_FALSE(...)                 \
  do{                                            \
    static_assert(!(__VA_ARGS__));               \
    ASSERT_FALSE(__VA_ARGS__);                   \
  } while(false)
