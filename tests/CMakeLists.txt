cmake_minimum_required(VERSION 3.16.3)

project("Bencode_Lib Unit Tests" VERSION 1.0.0 DESCRIPTION "Bencode C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests
enable_testing()

set(TEST_EXECUTABLE ${BENCODE_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
  ./bnode/Bencode_Lib_Tests_BNode.cpp
  ./bencode/Bencode_Lib_Tests_Bencode.cpp
  ./decoder/Bencode_Lib_Tests_Decode.cpp
  ./encoder/Bencode_Lib_Tests_Encode.cpp
  ./encoder/Bencode_Lib_Tests_JSON_Encode.cpp
  ./encoder/Bencode_Lib_Tests_XML_Encode.cpp
  ./IO/Bencode_Lib_Tests_ISource.cpp
  ./IO/Bencode_Lib_Tests_IDestination.cpp
  ./misc/Bencode_Lib_Tests_Helper.cpp
  ./misc/Bencode_Lib_Tests_Misc.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${TEST_EXECUTABLE} ${BENCODE_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files $<CONFIG>/files)