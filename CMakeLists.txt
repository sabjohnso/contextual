cmake_minimum_required(VERSION 3.12)

project(contextual VERSION 0.1.0 LANGUAGES C CXX)

option(contextual_BUILD_TESTING "Build the contextual tests" ON)

include(GNUInstallDirs)
set(contextual_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(contextual_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(contextual_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
set(contextual_INSTALL_CONFDIR ${contextual_INSTALL_LIBDIR}/cmake/contextual-${PROJECT_VERSION})

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(contextual_deps)

add_subdirectory(contextual)

if(contextual_BUILD_TESTING)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(contextual_testing)
  endif()
endif()

install(EXPORT contextual_EXPORTS
  NAMESPACE contextual::
  FILE contextual-exports.cmake
  DESTINATION ${contextual_INSTALL_CONFDIR})

configure_file(contextual-config.cmake.in contextual-config.cmake)
install(FILES ${PROJECT_BINARY_DIR}/contextual-config.cmake
  DESTINATION ${contextual_INSTALL_CONFDIR})








