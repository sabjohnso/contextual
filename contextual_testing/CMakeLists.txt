find_package(GTest)

macro(contextual_add_test test_name exe_name)
  add_executable(${exe_name} ${ARGN})
  target_link_libraries(${exe_name} PRIVATE
    contextual::header
    type_utility::header
    list_processing::header
    GTest::gtest_main)
  set_target_properties(${exe_name}
    PROPERTIES
    CXX_STANDARD ${contextual_CXX_STANDARD})
  add_test(${test_name} ${exe_name})
endmacro()

contextual_add_test(derive derive_test derive_test.cpp)
contextual_add_test(contextual contextual_test contextual_test.cpp)
contextual_add_test(magma magma_test magma_test.cpp)
contextual_add_test(semigroup semigroup_test semigroup_test.cpp)
contextual_add_test(monoid monoid_test monoid_test.cpp)
contextual_add_test(functor functor_test functor_test.cpp)
contextual_add_test(applicative applicative_test applicative_test.cpp)
contextual_add_test(monad monad_test monad_test.cpp)
contextual_add_test(monad_fail monad_fail_test monad_fail_test.cpp)
contextual_add_test(monad_plus monad_plus_test monad_plus_test.cpp)
contextual_add_test(monad_reader monad_reader_test monad_reader_test.cpp)
contextual_add_test(monad_state monad_state_test monad_state_test.cpp)
contextual_add_test(monad_writer monad_writer_test monad_writer_test.cpp)
contextual_add_test(monad_rws monad_rws_test monad_rws_test.cpp)
contextual_add_test(category category_test category_test.cpp)
contextual_add_test(type_context type_context_test type_context_test.cpp)
contextual_add_test(list_context list_context_test list_context_test.cpp)
contextual_add_test(thunk_context thunk_context_test thunk_context_test.cpp)
contextual_add_test(function_context function_context_test function_context_test.cpp)


find_package(Catch2 REQUIRED)

function(contextual_add_catch2_test name)
  set(exe_name ${name}_test)
  set(src_name ${exe_name}.cpp)
  add_executable(${exe_name} ${src_name})
  target_link_libraries(${exe_name}
    PRIVATE
    contextual::header
    type_utility::header
    list_processing::header
    Catch2::Catch2WithMain)
  set_target_properties(${exe_name}
    PROPERTIES
    CXX_STANDARD ${contextual_CXX_STANDARD})
  add_test(${name} ${exe_name})
endfunction()

# contextual_add_catch2_test(arrow)
