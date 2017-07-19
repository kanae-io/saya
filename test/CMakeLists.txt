include("${CMAKE_CURRENT_LIST_DIR}/../cmake/saya-test-generator.cmake")

set(test_flags "SAYA_TESTING=1;UNISTR_FROM_STRING_EXPLICIT=explicit")

set(include_dirs "")
list(
  APPEND include_dirs
  "${PROJECT_SOURCE_DIR}/include"
)
set(system_include_dirs "")
list(
  APPEND system_include_dirs
  ${Boost_INCLUDE_DIRS}
  ${ICU_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS}
)

set(SAYA_GTEST_ROOT "${CMAKE_CURRENT_LIST_DIR}/../modules/googletest")
saya_enable_gtest(
  "${SAYA_GTEST_ROOT}"
  gtest_main.cpp
  "${include_dirs}"
  "${system_include_dirs}"
  "${test_flags}"
)

file(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "*_test.cpp")

set(libs "")
list(
  APPEND libs
  ${Boost_LIBRARIES}
  ${ICU_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

saya_generate_tests("${SAYA_GTEST_ROOT}" "${files}" "${include_dirs}" "${system_include_dirs}" "${libs}" "${test_flags}")

