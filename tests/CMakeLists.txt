cmake_minimum_required(VERSION 3.16.3)

project("Bencode_Lib Unit Tests" VERSION 1.0.0 DESCRIPTION "Bencode C++ Library Catch Unit Tests" LANGUAGES CXX)

# Get Catch2 from repository
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests
enable_testing()

set(TEST_EXECUTABLE ${BENCODE_LIBRARY_NAME}_Unit_Tests)

set(TEST_SOURCES
        source/bnode/Bencode_Lib_Tests_BNode_Assignment.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Constructor.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Indexing.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Reference.cpp
        source/bencode/Bencode_Lib_Tests_Bencode_Complex.cpp
        source/bencode/Bencode_Lib_Tests_Bencode_Dictionary.cpp
        source/bencode/Bencode_Lib_Tests_Bencode_List.cpp
        source/decoder/Bencode_Lib_Tests_Decode_Collection.cpp
        source/decoder/Bencode_Lib_Tests_Decode_Exception.cpp
        source/decoder/Bencode_Lib_Tests_Decode_Misc.cpp
        source/decoder/Bencode_Lib_Tests_Decode_Simple.cpp
        source/encoder/Bencode_Lib_Tests_Encode_Collection.cpp
        source/encoder/Bencode_Lib_Tests_Encode_Simple.cpp
        source/encoder/Bencode_Lib_Tests_Encode_Misc.cpp
        source/encoder/Bencode_Lib_Tests_JSON_Encode.cpp
        source/encoder/Bencode_Lib_Tests_XML_Encode.cpp
        source/io/Bencode_Lib_Tests_ISource_Buffer.cpp
        source/io/Bencode_Lib_Tests_ISource_File.cpp
        source/io/Bencode_Lib_Tests_IDestination_Buffer.cpp
        source/io/Bencode_Lib_Tests_IDestination_File.cpp
        source/misc/Bencode_Lib_Tests_Helper.cpp
        source/misc/Bencode_Lib_Tests_Misc.cpp
        source/traverse/Bencode_lib_Tests_Traverse.cpp)

add_executable(${TEST_EXECUTABLE} ${TEST_SOURCES})
target_include_directories(${TEST_EXECUTABLE} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${TEST_EXECUTABLE} ${BENCODE_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)