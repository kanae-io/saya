set(SAYA_IKA_COMPILE_DEFINITIONS "BOOST_SPIRIT_USE_PHOENIX_V3=1")

add_subdirectory(grammar)

# ------------------------------------------------------

add_library(saya_ika_compiler STATIC "compiler_misc.cpp" "compiler.cpp")

target_include_directories(
  saya_ika_compiler
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_compiler PUBLIC
  saya_ika_grammar
)

# ------------------------------------------------------

add_library(saya_ika_evaluator STATIC "evaluator.cpp")

target_include_directories(
  saya_ika_evaluator
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_evaluator PUBLIC
  saya
)

# ------------------------------------------------------

add_library(saya_ika_generator STATIC "generator.cpp")

target_include_directories(
  saya_ika_generator
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_generator PUBLIC
  saya
)

# ------------------------------------------------------

add_executable(saya_ika_ikac "ikac.cpp")

set_target_properties(
  saya_ika_ikac PROPERTIES
  OUTPUT_NAME "ikac"
  COMPILE_DEFINITIONS "${SAYA_IKA_COMPILE_DEFINITIONS}"
)

target_include_directories(
  saya_ika_ikac
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_ikac PUBLIC
  saya_ika_compiler
  saya_ika_evaluator
  saya_ika_generator
)

install(
  TARGETS saya_ika_ikac
  EXPORT saya-export
  RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
)

