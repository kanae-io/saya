include("${PROJECT_SOURCE_DIR}/cmake/saya-test-generator.cmake")

set(test_flags "SAYA_TESTING=1;UNISTR_FROM_STRING_EXPLICIT=explicit")

set(include_dirs "")
list(
  APPEND include_dirs
  "${PROJECT_SOURCE_DIR}/include"
  "include"
)
set(system_include_dirs "")
list(
  APPEND system_include_dirs
  ${Boost_INCLUDE_DIRS}
  ${ICU_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS}
)

set(SAYA_GTEST_ROOT "${PROJECT_SOURCE_DIR}/modules/googletest")
saya_enable_gtest(
  "${SAYA_GTEST_ROOT}"
  gtest_main.cpp
  "${include_dirs}"
  "${system_include_dirs}"
  "${test_flags}"
)

# -------------------------------------------------------
macro(resolve_tests files)
  set(tests "")
  foreach(f IN ITEMS "${files}")
    list(APPEND tests "${f}_test.cpp")
  endforeach()
endmacro()

resolve_tests(
  "logger"
  "syncstream"
)

set(
  libs
  ${Boost_LIBRARIES}
  ${ICU_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

saya_generate_tests("${SAYA_GTEST_ROOT}" "${tests}" "${include_dirs}" "${system_include_dirs}" "${libs}" "${test_flags}")
