cmake_minimum_required(VERSION 3.18.1)

project("Bencode_Lib" VERSION 1.1.0 DESCRIPTION "Bencode C++ Library" LANGUAGES CXX)

# C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(MSVC)
  add_compile_options(/W4)
  set(PLATFORM "windows")
else()
  add_compile_options(-Wall -Werror -pedantic)
  set(PLATFORM "linux")
endif()

# Setup library version
configure_file(Bencode_Config.h.in Bencode_Config.hpp)

# Library name
set(BENCODE_LIBRARY_NAME "${PROJECT_NAME}")

# Bencode sources and includes
set(BENCODE_SOURCES
  ./classes/source/Bencode.cpp
  ./classes/source/implementation/Bencode_Impl.cpp
  ./classes/source/implementation/decoder/Bencode_Decoder.cpp)

set(BENCODE_INCLUDES
  ./classes/include/Bencode.hpp
  ./classes/include/interface/Bencode_Interfaces.hpp
  ./classes/include/interface/IAction.hpp
  ./classes/include/interface/ISource.hpp
  ./classes/include/interface/IDestination.hpp
  ./classes/include/interface/IDecoder.hpp
  ./classes/include/interface/IEncoder.hpp
  ./classes/include/interface/ITranslator.hpp
  ./classes/include/implementation/Bencode_Core.hpp
  ./classes/include/implementation/common/Bencode_Error.hpp
  ./classes/include/implementation/bnode/Bencode_BNode_Creation.hpp
  ./classes/include/implementation/bnode/Bencode_BNode_Index.hpp
  ./classes/include/implementation/bnode/Bencode_BNode_Reference.hpp
  ./classes/include/implementation/bnode/Bencode_BNode.hpp
  ./classes/include/implementation/Bencode_Impl.hpp
  ./classes/include/implementation/decoder/Bencode_Decoder.hpp
  ./classes/include/implementation/encoder/Bencode_Encoder.hpp
  ./classes/include/implementation/encoder/JSON_Encoder.hpp
  ./classes/include/implementation/encoder/XML_Encoder.hpp
  ./classes/include/implementation/encoder/YAML_Encoder.hpp
  ./classes/include/implementation/translator/JSON_Translator.hpp
  ./classes/include/implementation/translator/XML_Translator.hpp
  ./classes/include/implementation/io/Bencode_Sources.hpp
  ./classes/include/implementation/io/Bencode_BufferSource.hpp
  ./classes/include/implementation/io/Bencode_FileSource.hpp
  ./classes/include/implementation/io/Bencode_Destinations.hpp
  ./classes/include/implementation/io/Bencode_BufferDestination.hpp
  ./classes/include/implementation/io/Bencode_FileDestination.hpp
  ./classes/include/implementation/variants/Bencode_Dictionary.hpp
  ./classes/include/implementation/variants/Bencode_Hole.hpp
  ./classes/include/implementation/variants/Bencode_Integer.hpp
  ./classes/include/implementation/variants/Bencode_List.hpp
  ./classes/include/implementation/variants/Bencode_String.hpp
  ./classes/include/implementation/variants/Bencode_Variant.hpp)

# Bencode library
add_library(${BENCODE_LIBRARY_NAME} ${BENCODE_SOURCES} ${BENCODE_INCLUDES})
target_include_directories(${BENCODE_LIBRARY_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/classes/include
  ${PROJECT_SOURCE_DIR}/classes/include/interface
  ${PROJECT_SOURCE_DIR}/classes/include/implementation
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/common
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/bnode
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/decoder
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/encoder
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/io
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/variants
  ${PROJECT_SOURCE_DIR}/classes/include/implementation/translator
  ${PROJECT_BINARY_DIR})

# Add tests
add_subdirectory(tests)

# Add examples
add_subdirectory(examples)

# install
install(TARGETS ${BENCODE_LIBRARY_NAME} DESTINATION lib)
install(FILES ${BENCODE_INCLUDES} DESTINATION include)
