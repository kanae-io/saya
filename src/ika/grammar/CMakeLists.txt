add_library(saya_ika_grammar STATIC "to_ast.cpp")

set(defs
  "BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS"
  "BOOST_MPL_LIMIT_LIST_SIZE=30"
)

set_target_properties(
  saya_ika_grammar PROPERTIES
  COMPILE_DEFINITIONS "${SAYA_IKA_COMPILE_DEFINITIONS};${defs}"
)

target_include_directories(
  saya_ika_grammar
  SYSTEM BEFORE PUBLIC "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(
  saya_ika_grammar PUBLIC
  saya
)

