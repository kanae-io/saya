function(saya_remove_gtest_warnings target)
  set(flags "")
  if(NOT MSVC)
    set(fags "-Wno-missing-field-initializers")
  endif()
  set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${flags}")
endfunction()

function(saya_enable_gtest gtest_source_dir test_entrypoint_src include_dirs system_include_dirs compile_definitions)
  if(NOT TARGET all_tests)
    add_custom_target(all_tests)
  endif()

  set(
    gtest_include_dirs
    "${gtest_source_dir}/googlemock"
    "${gtest_source_dir}/googlemock/include"
    "${gtest_source_dir}/googletest"
    "${gtest_source_dir}/googletest/include"
  )

  add_library(
    "${PROJECT_NAME}_gtest" STATIC
    "${gtest_source_dir}/googletest/src/gtest-all.cc"
  )

  target_include_directories(
    "${PROJECT_NAME}_gtest" SYSTEM BEFORE PUBLIC
    ${gtest_include_dirs}
  )

  set_target_properties(
    "${PROJECT_NAME}_gtest" PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test"
  )

  saya_remove_gtest_warnings("${PROJECT_NAME}_gtest")

  # ----------------------------------

  add_library("${PROJECT_NAME}_gtest_main" STATIC ${test_entrypoint_src})

  target_include_directories(
    "${PROJECT_NAME}_gtest_main" SYSTEM BEFORE PUBLIC
    ${gtest_include_dirs}
  )
  target_include_directories(
    "${PROJECT_NAME}_gtest_main" BEFORE PUBLIC
    ${include_dirs}
  )
  target_include_directories(
    "${PROJECT_NAME}_gtest_main" SYSTEM BEFORE PUBLIC
    ${system_include_dirs}
  )

  set_target_properties(
    "${PROJECT_NAME}_gtest_main" PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test"
    COMPILE_DEFINITIONS "${compile_definitions}"
  )
  saya_remove_gtest_warnings("${PROJECT_NAME}_gtest_main")
endfunction()

function(saya_generate_tests gtest_source_dir test_files include_dirs system_include_dirs libs compile_definitions)
  set(
    gtest_include_dirs
    "${gtest_source_dir}/googlemock"
    "${gtest_source_dir}/googlemock/include"
    "${gtest_source_dir}/googletest"
    "${gtest_source_dir}/googletest/include"
  )

  foreach(test_file ${test_files})
    set(test_target_name ${test_file})
    string(REGEX REPLACE "\\.cpp$" "" test_target_name ${test_target_name})
    string(REGEX REPLACE "/" "_" test_target_name ${test_target_name})

    set(test_executable_name "${test_target_name}")
    add_executable(${test_target_name} ${test_file})

    target_link_libraries(
      ${test_target_name}
      "${PROJECT_NAME}_gtest_main"
      "${PROJECT_NAME}_gtest"
      ${libs}
    )

    target_include_directories(
      ${test_target_name} SYSTEM BEFORE PUBLIC
      ${gtest_include_dirs}
    )
    target_include_directories(
      ${test_target_name} BEFORE PUBLIC
      ${include_dirs}
    )
    target_include_directories(
      ${test_target_name} SYSTEM BEFORE PUBLIC
      ${system_include_dirs}
    )

    set_target_properties(
      ${test_target_name} PROPERTIES
      OUTPUT_NAME ${test_executable_name}
      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test"
      COMPILE_DEFINITIONS "${compile_definitions}"
    )
    add_test(
      ${test_target_name}
      ${test_executable_name}
      --gtest_color=yes
    )
    add_dependencies(all_tests ${test_target_name})
  endforeach()
endfunction()

