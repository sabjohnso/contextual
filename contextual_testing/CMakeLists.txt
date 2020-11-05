find_package(GTest)

macro(contextual_add_test test_name exe_name)
  add_executable(${exe_name} ${ARGN})
  target_link_libraries(${exe_name} PRIVATE
    contextual::header
    GTest::gtest_main)
  add_test(${test_name} ${exe_name})
endmacro()
