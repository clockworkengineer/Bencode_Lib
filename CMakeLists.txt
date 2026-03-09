cmake_minimum_required(VERSION 3.21)

project("Bencode_Lib" VERSION 1.1.0 DESCRIPTION "Bencode C++ Library" LANGUAGES CXX)

# C++ options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BENCODE_BUILD_TESTS "Build unit tests" ON)
option(BENCODE_BUILD_EXAMPLES "Build examples" ON)
option(BENCODE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Setup library version
configure_file(Bencode_Config.h.in Bencode_Config.hpp @ONLY)

# Library name
set(BENCODE_LIBRARY_NAME "${PROJECT_NAME}")

# Bencode sources and includes
set(BENCODE_SOURCES
  classes/source/Bencode.cpp
  classes/source/implementation/Bencode_Impl.cpp
  classes/source/implementation/file/Bencode_File.cpp
  classes/source/implementation/parser/Default_Parser.cpp
)

set(BENCODE_INCLUDES
  classes/include/Bencode.hpp
  classes/include/Bencode_Core.hpp
  classes/include/interface/Bencode_Interfaces.hpp
  classes/include/interface/IAction.hpp
  classes/include/interface/ISource.hpp
  classes/include/interface/IDestination.hpp
  classes/include/interface/IParser.hpp
  classes/include/interface/IStringify.hpp
  classes/include/interface/ITranslator.hpp
  classes/include/implementation/common/Bencode_Error.hpp
  classes/include/implementation/node/Bencode_Node_Creation.hpp
  classes/include/implementation/node/Bencode_Node_Index.hpp
  classes/include/implementation/node/Bencode_Node_Reference.hpp
  classes/include/implementation/node/Bencode_Node.hpp
  classes/include/implementation/Bencode_Impl.hpp
  classes/include/implementation/parser/Default_Parser.hpp
  classes/include/implementation/stringify/Default_Stringify.hpp
  classes/include/implementation/stringify/JSON_Stringify.hpp
  classes/include/implementation/stringify/XML_Stringify.hpp
  classes/include/implementation/stringify/YAML_Stringify.hpp
  classes/include/implementation/translator/Default_Translator.hpp
  classes/include/implementation/translator/XML_Translator.hpp
  classes/include/implementation/io/Bencode_Sources.hpp
  classes/include/implementation/io/Bencode_BufferSource.hpp
  classes/include/implementation/io/Bencode_FileSource.hpp
  classes/include/implementation/io/Bencode_Destinations.hpp
  classes/include/implementation/io/Bencode_BufferDestination.hpp
  classes/include/implementation/io/Bencode_FileDestination.hpp
  classes/include/implementation/variants/Bencode_Dictionary.hpp
  classes/include/implementation/variants/Bencode_Hole.hpp
  classes/include/implementation/variants/Bencode_Integer.hpp
  classes/include/implementation/variants/Bencode_List.hpp
  classes/include/implementation/variants/Bencode_String.hpp
  classes/include/implementation/variants/Bencode_Variant.hpp
)

# Bencode library target
add_library(${BENCODE_LIBRARY_NAME} ${BENCODE_SOURCES} ${BENCODE_INCLUDES})

# Namespaced alias for consistent use in examples, tests, and by consumers
add_library(Bencode_Lib::Bencode_Lib ALIAS ${BENCODE_LIBRARY_NAME})

target_include_directories(${BENCODE_LIBRARY_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/interface>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/common>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/node>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/parser>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/stringify>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/io>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/variants>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/classes/include/implementation/translator>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)

# Target-scoped warnings (replaces global add_compile_options)
if(MSVC)
  target_compile_options(${BENCODE_LIBRARY_NAME} PRIVATE
    /W4
    /permissive-
    /Zc:__cplusplus
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
  )
else()
  target_compile_options(${BENCODE_LIBRARY_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
  )
endif()

# Pre-compiled headers (PRIVATE so consumers are not forced to use it)
target_precompile_headers(${BENCODE_LIBRARY_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h
)

# Tests
include(CTest)

if(BENCODE_BUILD_TESTS AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Examples
if(BENCODE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Install rules
include(GNUInstallDirs)

install(TARGETS ${BENCODE_LIBRARY_NAME}
  EXPORT BencodeTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers preserving directory structure
install(DIRECTORY classes/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install generated config header
install(FILES ${PROJECT_BINARY_DIR}/Bencode_Config.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets for downstream CMake consumers
install(EXPORT BencodeTargets
  FILE BencodeTargets.cmake
  NAMESPACE Bencode_Lib::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Bencode_Lib
)
