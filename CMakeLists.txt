cmake_minimum_required(VERSION 3.16.3)

project("Bencode_Lib" VERSION 1.0.0 DESCRIPTION "Bencode C++ Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

configure_file(Bencode_Config.h.in Bencode_Config.hpp)

set(BENCODE_LIBRARY_NAME "${PROJECT_NAME}")

set(BENCODE_SOURCES
  ./classes/Bencode.cpp
  ./classes/implementation/Bencode_Impl.cpp
  ./classes/implementation/decoder/Bencode_Decoder.cpp
  ./classes/implementation/encoder/Bencode_Encoder.cpp)

set(BENCODE_INCLUDES
  ./include/Bencode.hpp
  ./include/interface/Bencode_Interfaces.hpp
  ./include/interface/IAction.hpp
  ./include/interface/ISource.hpp
  ./include/interface/IDestination.hpp
  ./include/interface/IDecoder.hpp
  ./include/interface/IEncoder.hpp
  ./include/implementation/Bencode_Core.hpp
  ./include/implementation/bnode/Bencode_BNode_Creation.hpp
  ./include/implementation/bnode/Bencode_BNode_Index.hpp
  ./include/implementation/bnode/Bencode_BNode_Reference.hpp
  ./include/implementation/bnode/Bencode_BNode.hpp
  ./include/implementation/Bencode_Impl.hpp
  ./include/implementation/decoder/Bencode_Decoder.hpp
  ./include/implementation/encoder/Bencode_Encoder.hpp
  ./include/implementation/encoder/JSON_Encoder.hpp
  ./include/implementation/encoder/XML_Encoder.hpp
  ./include/implementation/translator/JSON_Translator.hpp
  ./include/implementation/translator/XML_Translator.hpp
  ./include/implementation/io/Bencode_Sources.hpp
  ./include/implementation/io/Bencode_BufferSource.hpp
  ./include/implementation/io/Bencode_FileSource.hpp
  ./include/implementation/io/Bencode_Destinations.hpp
  ./include/implementation/io/Bencode_BufferDestination.hpp
  ./include/implementation/io/Bencode_FileDestination.hpp
  ./include/implementation/variants/Bencode_Dictionary.hpp
  ./include/implementation/variants/Bencode_Hole.hpp
  ./include/implementation/variants/Bencode_Integer.hpp
  ./include/implementation/variants/Bencode_List.hpp
  ./include/implementation/variants/Bencode_String.hpp
  ./include/implementation/variants/Bencode_Variant.hpp)

# Bencode library
add_library(${BENCODE_LIBRARY_NAME} ${BENCODE_SOURCES} ${BENCODE_INCLUDES})
target_include_directories(${BENCODE_LIBRARY_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/interface
  ${PROJECT_SOURCE_DIR}/include/implementation
  ${PROJECT_SOURCE_DIR}/include/implementation/bnode
  ${PROJECT_SOURCE_DIR}/include/implementation/decoder
  ${PROJECT_SOURCE_DIR}/include/implementation/encoder
  ${PROJECT_SOURCE_DIR}/include/implementation/io
  ${PROJECT_SOURCE_DIR}/include/implementation/variants
  ${PROJECT_SOURCE_DIR}/include/implementation/translator
  ${PROJECT_SOURCE_DIR}/include/external
  ${PROJECT_BINARY_DIR})

# Add tests
add_subdirectory(tests)

# Add examples
add_subdirectory(examples)

# install
install(TARGETS ${BENCODE_LIBRARY_NAME} DESTINATION lib)
install(FILES ${BENCODE_INCLUDES} DESTINATION include)
