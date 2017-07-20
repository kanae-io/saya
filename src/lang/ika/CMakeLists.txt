file(GLOB_RECURSE SAYA_IKA_AST_SRCS "ast" "*.cpp")
add_library(saya_ika_ast STATIC ${SAYA_IKA_AST_SRCS})
target_link_libraries(saya_ika_ast PUBLIC saya)
install(
  TARGETS saya_ika_ast
  EXPORT saya-export
  ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/lib"
)
