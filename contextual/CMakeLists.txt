configure_file(config.hpp.in config.hpp)

add_library(contextual_header INTERFACE)
set_target_properties(contextual_header PROPERTIES EXPORT_NAME header)
target_include_directories(contextual_header
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${contextual_INSTALL_INCLUDEDIR}>)

target_link_libraries(contextual_header INTERFACE
  type_utility::header
  function_utility::function_utility
  ctstring::header
  list_processing::header)

add_library(contextual::header ALIAS contextual_header)

install(TARGETS contextual_header
  EXPORT contextual_EXPORTS
  RUNTIME DESTINATION ${contextual_INSTALL_BINDIR}
  LIBRARY DESTINATION ${contextual_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${contextual_INSTALL_LIBDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
  Monad.hpp
  Derive.hpp
  MonadPlus.hpp
  Monoid.hpp
  Run.hpp
  Unit.hpp
  DESTINATION ${contextual_INSTALL_INCLUDEDIR}/contextual)

install(DIRECTORY
  details
  instances
  DESTINATION ${contextual_INSTALL_INCLUDEDIR}/contextual
  FILES_MATCHING PATTERN "*.hpp")
