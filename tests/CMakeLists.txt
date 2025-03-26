cmake_minimum_required(VERSION 3.18.1)

project("Bencode_Lib Unit Tests" VERSION 1.1.0 DESCRIPTION "Bencode C++ Library Catch Unit Tests" LANGUAGES CXX)

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
        source/bnode/Bencode_Lib_Tests_BNode_Assignment.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Constructor.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Indexing.cpp
        source/bnode/Bencode_Lib_Tests_BNode_Reference.cpp
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
target_link_libraries(${TEST_EXECUTABLE} ${BENCODE_LIBRARY_NAME} Catch2::Catch2WithMain)

add_test(NAME ${TEST_EXECUTABLE} COMMAND ${TEST_EXECUTABLE})

add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files files)
add_custom_command(TARGET ${TEST_EXECUTABLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)