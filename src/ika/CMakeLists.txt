set(SAYA_IKA_COMPILE_DEFINITIONS "BOOST_SPIRIT_USE_PHOENIX_V3=1")

add_subdirectory(grammar)

add_executable(saya_ika_compiler "ikac.cpp" "evaluator.cpp")

set_target_properties(
  saya_ika_compiler PROPERTIES
  OUTPUT_NAME "ikac"
  COMPILE_DEFINITIONS "${SAYA_IKA_COMPILE_DEFINITIONS}"
)

target_include_directories(
  saya_ika_compiler
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_compiler PUBLIC
  saya_ika_grammar
  saya
)

install(
  TARGETS saya_ika_compiler
  EXPORT saya-export
  RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
)

