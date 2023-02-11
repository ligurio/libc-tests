add_library(fuzzer_config INTERFACE)

target_compile_options(
    fuzzer_config
    INTERFACE
        $<$<NOT:$<BOOL:${OSS_FUZZ}>>:
        -fsanitize=fuzzer
        >
        $<$<BOOL:${OSS_FUZZ}>:
        ${CXX}
        ${CXXFLAGS}
        >
)
target_link_libraries(
    fuzzer_config
    INTERFACE
        $<$<NOT:$<BOOL:${OSS_FUZZ}>>:
        -fsanitize=fuzzer
        >
        $<$<BOOL:${OSS_FUZZ}>:
        $ENV{LIB_FUZZING_ENGINE}
        >
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(LDFLAGS "${LDFLAGS} ${CMAKE_C_FLAGS_DEBUG}")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(FUZZ_TEST_TARGETS "")

include(CheckFunctionExists)

function(create_test)
  cmake_parse_arguments(
    FUZZ
    ""
    "FILENAME"
    "SOURCES;LIBRARIES"
    ${ARGN}
  )

  check_function_exists(${FUZZ_FILENAME} HAVE_${FUZZ_FILENAME})
  if (NOT ${HAVE_${FUZZ_FILENAME}})
      message(WARNING "Function ${FUZZ_FILENAME} is missed")
      return()
  endif ()

  get_filename_component(test_name ${FUZZ_FILENAME} NAME_WE)
  add_executable(${test_name} ${FUZZ_SOURCES})

  target_link_libraries(${test_name} PUBLIC fuzzer_config ${FUZZ_LIBRARIES} ${LDFLAGS})
  target_compile_options(${test_name} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter -g)
  string(REPLACE "_test" "" test_prefix ${test_name})
  add_test(NAME ${test_name}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test_name}
                   -use_value_profile=1
                   -report_slow_units=5
                   -reload=1
                   -reduce_inputs=1
                   -print_pcs=1
                   -print_final_stats=1
                   -mutate_depth=20
                   -runs=5
                   -artifact_prefix=${test_name}_
  )
endfunction()

file(GLOB tests LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cc)
foreach(filename ${tests})
  get_filename_component(test_name ${filename} NAME_WE)
  create_test(FILENAME ${test_name}
              SOURCES ${filename}
              LIBRARIES "")
endforeach()
