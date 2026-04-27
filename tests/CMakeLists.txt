cmake_minimum_required(VERSION 3.18.1)

project("Bencode_Lib Unit Tests" VERSION 1.2.0 DESCRIPTION "Bencode C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests
enable_testing()

set(TEST_EXECUTABLE ${BENCODE_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
  source/node/Bencode_Lib_Tests_Node_Assignment.cpp
  source/node/Bencode_Lib_Tests_Node_Constructor.cpp
  source/node/Bencode_Lib_Tests_Node_Indexing.cpp
  source/node/Bencode_Lib_Tests_Node_Reference.cpp
  source/bencode/Bencode_Lib_Tests_Bencode_Complex.cpp
  source/bencode/Bencode_Lib_Tests_Bencode_Dictionary.cpp
  source/bencode/Bencode_Lib_Tests_Bencode_List.cpp
  source/parser/Bencode_Lib_Tests_Parse_Collection.cpp
  source/parser/Bencode_Lib_Tests_Parse_Exception.cpp
  source/parser/Bencode_Lib_Tests_Parse_Misc.cpp
  source/parser/Bencode_Lib_Tests_Parse_Simple.cpp
  source/stringify/Bencode_Lib_Tests_Stringify_Collection.cpp
  source/stringify/Bencode_Lib_Tests_Stringify_Simple.cpp
  source/stringify/Bencode_Lib_Tests_Stringify_Misc.cpp
  source/stringify/Bencode_Lib_Tests_JSON_Stringify.cpp
  source/stringify/Bencode_Lib_Tests_XML_Stringify.cpp
  source/io/Bencode_Lib_Tests_ISource_Buffer.cpp
  source/io/Bencode_Lib_Tests_ISource_File.cpp
  source/io/Bencode_Lib_Tests_IDestination_Buffer.cpp
  source/io/Bencode_Lib_Tests_IDestination_File.cpp
  source/misc/Bencode_Lib_Tests_Helper.cpp
  source/misc/Bencode_Lib_Tests_Misc.cpp
  source/traverse/Bencode_lib_Tests_Traverse.cpp
  source/stringify/Bencode_Lib_Tests_YAML_Stringify.cpp
  source/io/Bencode_Tests_File_FromFile.cpp
  source/io/Bencode_Lib_Tests_ToFile.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${TEST_EXECUTABLE} Bencode_Lib::Bencode_Lib Catch2::Catch2WithMain)
target_precompile_headers(${TEST_EXECUTABLE} REUSE_FROM Bencode_Lib)

# Embedded test suite: compile the library and a minimal embedded-focused test harness
set(EMBEDDED_TEST_EXECUTABLE ${BENCODE_LIBRARY_NAME}_Embedded_Unit_Tests)
add_executable(${EMBEDDED_TEST_EXECUTABLE} source/embedded/Bencode_Lib_Tests_Embedded.cpp)
target_include_directories(${EMBEDDED_TEST_EXECUTABLE} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${EMBEDDED_TEST_EXECUTABLE} Bencode_Lib::Bencode_Lib_Embedded Catch2::Catch2WithMain)
target_precompile_headers(${EMBEDDED_TEST_EXECUTABLE} PRIVATE ${CMAKE_SOURCE_DIR}/stdafx.h)
target_compile_definitions(${EMBEDDED_TEST_EXECUTABLE} PRIVATE
  BENCODE_EMBEDDED_MODE
  BENCODE_ENABLE_EXCEPTIONS=0
  BENCODE_ENABLE_FILE_IO=0
  BENCODE_ENABLE_DYNAMIC_ALLOCATION=0
)
add_test(NAME ${EMBEDDED_TEST_EXECUTABLE} COMMAND ${EMBEDDED_TEST_EXECUTABLE})

if(BENCODE_BUILD_MINIMAL)
  set(MINIMAL_TEST_EXECUTABLE ${BENCODE_LIBRARY_NAME}_Minimal_Unit_Tests)
  add_executable(${MINIMAL_TEST_EXECUTABLE} source/minimal/Bencode_Lib_Tests_Minimal.cpp)
  target_include_directories(${MINIMAL_TEST_EXECUTABLE} PUBLIC ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(${MINIMAL_TEST_EXECUTABLE} Bencode_Lib::Bencode_Lib_Minimal Catch2::Catch2WithMain)
  target_precompile_headers(${MINIMAL_TEST_EXECUTABLE} PRIVATE ${CMAKE_SOURCE_DIR}/stdafx.h)
  add_test(NAME ${MINIMAL_TEST_EXECUTABLE} COMMAND ${MINIMAL_TEST_EXECUTABLE})
endif()

if(MSVC)
  target_compile_options(${TEST_EXECUTABLE} PRIVATE
    /W4
    /permissive-
    /Zc:__cplusplus
    /wd4324 # Catch2: structure padded due to alignment specifier
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
  )
  target_compile_options(${EMBEDDED_TEST_EXECUTABLE} PRIVATE
    /W4
    /permissive-
    /Zc:__cplusplus
    /wd4324 # Catch2: structure padded due to alignment specifier
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
  )

  if(BENCODE_BUILD_MINIMAL)
    target_compile_options(${MINIMAL_TEST_EXECUTABLE} PRIVATE
      /W4
      /permissive-
      /Zc:__cplusplus
      /wd4324 # Catch2: structure padded due to alignment specifier
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
    )
  endif()
else()
  target_compile_options(${TEST_EXECUTABLE} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
  )
  target_compile_options(${EMBEDDED_TEST_EXECUTABLE} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
  )

  if(BENCODE_BUILD_MINIMAL)
    target_compile_options(${MINIMAL_TEST_EXECUTABLE} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
    )
  endif()
endif()

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

if(BENCODE_BUILD_BENCHMARKS)
  add_executable(Bencode_Lib_Benchmark source/benchmark/Bencode_Lib_Benchmark.cpp)
  target_include_directories(Bencode_Lib_Benchmark PUBLIC ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(Bencode_Lib_Benchmark Bencode_Lib::Bencode_Lib)
  target_precompile_headers(Bencode_Lib_Benchmark REUSE_FROM Bencode_Lib)

  add_executable(Bencode_Lib_Embedded_Benchmark source/benchmark/Bencode_Lib_Benchmark.cpp)
  target_include_directories(Bencode_Lib_Embedded_Benchmark PUBLIC ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(Bencode_Lib_Embedded_Benchmark Bencode_Lib::Bencode_Lib_Embedded)
  target_precompile_headers(Bencode_Lib_Embedded_Benchmark PRIVATE ${CMAKE_SOURCE_DIR}/stdafx.h)
  target_compile_definitions(Bencode_Lib_Embedded_Benchmark PRIVATE
    BENCODE_EMBEDDED_MODE
    BENCODE_ENABLE_EXCEPTIONS=0
    BENCODE_ENABLE_FILE_IO=0
    BENCODE_ENABLE_DYNAMIC_ALLOCATION=0
  )

  if(MSVC)
    target_compile_options(Bencode_Lib_Benchmark PRIVATE
      /W4
      /permissive-
      /Zc:__cplusplus
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
    )
    target_compile_options(Bencode_Lib_Embedded_Benchmark PRIVATE
      /W4
      /permissive-
      /Zc:__cplusplus
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
    )
  else()
    target_compile_options(Bencode_Lib_Benchmark PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
    )
    target_compile_options(Bencode_Lib_Embedded_Benchmark PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
    )
  endif()

  add_test(NAME Bencode_Lib_Benchmark COMMAND Bencode_Lib_Benchmark)
  add_test(NAME Bencode_Lib_Embedded_Benchmark COMMAND Bencode_Lib_Embedded_Benchmark)
endif()

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files files)