cmake_minimum_required(VERSION 3.21)

project("Bencode_Lib" VERSION 1.2.0 DESCRIPTION "Bencode C++ Library" LANGUAGES CXX)

# C++ options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BENCODE_BUILD_TESTS "Build unit tests" ON)
option(BENCODE_BUILD_EXAMPLES "Build examples" ON)
option(BENCODE_BUILD_BENCHMARKS "Build performance benchmark executable" ON)
option(BENCODE_BUILD_MINIMAL "Build minimal library variant with core parse/stringify and buffer-based I/O" OFF)

# Minimal library variant is intended for consumer builds that want a smaller public surface
# and do not require file-based I/O or the optional stringifier extensions.
option(BENCODE_EMBEDDED_MODE "Enable embedded build mode" OFF)

# Embedded library alias uses fixed limits, no exceptions, no file I/O, and no dynamic allocation.
option(BENCODE_ENABLE_EXCEPTIONS "Enable exception support" ON)
option(BENCODE_ENABLE_FILE_IO "Enable file-based I/O support" ON)
option(BENCODE_ENABLE_DYNAMIC_ALLOCATION "Enable dynamic allocations for containers" ON)
set(BENCODE_MAX_NODE_COUNT 256 CACHE STRING "Maximum total node count in embedded mode")
set(BENCODE_MAX_CONTAINER_SIZE 64 CACHE STRING "Maximum number of elements in embedded lists/dictionaries")
set(BENCODE_MAX_STRING_LENGTH 16384 CACHE STRING "Maximum string length in embedded mode" FORCE)
option(BENCODE_ENABLE_JSON_STRINGIFY "Enable JSON stringify support" ON)
option(BENCODE_ENABLE_XML_STRINGIFY "Enable XML stringify support" ON)
option(BENCODE_ENABLE_YAML_STRINGIFY "Enable YAML stringify support" ON)
option(BENCODE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Setup library version
configure_file(Bencode_Config.h.in Bencode_Config.hpp @ONLY)

# Library name
set(BENCODE_LIBRARY_NAME "${PROJECT_NAME}")

# Bencode sources and includes
set(BENCODE_SOURCES
  classes/source/Bencode.cpp
  classes/source/implementation/Bencode_Impl.cpp
  classes/source/implementation/parser/Default_Parser.cpp
)

if(BENCODE_ENABLE_FILE_IO)
  # File-based I/O support is only compiled into the main library when enabled.
  list(APPEND BENCODE_SOURCES
    classes/source/implementation/file/Bencode_File.cpp
  )
endif()

set(BENCODE_MINIMAL_SOURCES
  classes/source/Bencode.cpp
  classes/source/implementation/Bencode_Impl.cpp
  classes/source/implementation/parser/Default_Parser.cpp
)

set(BENCODE_EMBEDDED_SOURCES
  classes/source/Bencode.cpp
  classes/source/implementation/Bencode_Impl.cpp
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
  classes/include/implementation/translator/Default_Translator.hpp
  classes/include/implementation/translator/XML_Translator.hpp
  classes/include/implementation/io/Bencode_Sources.hpp
  classes/include/implementation/io/Bencode_BufferSource.hpp
  classes/include/implementation/io/Bencode_Destinations.hpp
  classes/include/implementation/io/Bencode_BufferDestination.hpp
  classes/include/implementation/variants/Bencode_Dictionary.hpp
  classes/include/implementation/variants/Bencode_FixedVector.hpp
  classes/include/implementation/variants/Bencode_Hole.hpp
  classes/include/implementation/variants/Bencode_Integer.hpp
  classes/include/implementation/variants/Bencode_List.hpp
  classes/include/implementation/variants/Bencode_String.hpp
  classes/include/implementation/variants/Bencode_Variant.hpp
)

if(BENCODE_ENABLE_FILE_IO)
  list(APPEND BENCODE_INCLUDES
    classes/include/implementation/io/Bencode_FileSource.hpp
    classes/include/implementation/io/Bencode_FileDestination.hpp
  )
endif()

if(BENCODE_ENABLE_JSON_STRINGIFY)
  list(APPEND BENCODE_INCLUDES
    classes/include/implementation/stringify/JSON_Stringify.hpp
  )
endif()

if(BENCODE_ENABLE_XML_STRINGIFY)
  list(APPEND BENCODE_INCLUDES
    classes/include/implementation/stringify/XML_Stringify.hpp
  )
endif()

if(BENCODE_ENABLE_YAML_STRINGIFY)
  list(APPEND BENCODE_INCLUDES
    classes/include/implementation/stringify/YAML_Stringify.hpp
  )
endif()

# Bencode library target
add_library(${BENCODE_LIBRARY_NAME} ${BENCODE_SOURCES} ${BENCODE_INCLUDES})

# Minimal core-only library variant for consumers who do not need file-based I/O or optional stringifiers
if(BENCODE_BUILD_MINIMAL)
  add_library(${BENCODE_LIBRARY_NAME}_Minimal ${BENCODE_MINIMAL_SOURCES} ${BENCODE_INCLUDES})
  add_library(Bencode_Lib::Bencode_Lib_Minimal ALIAS ${BENCODE_LIBRARY_NAME}_Minimal)
endif()

# Embedded library variant for examples and embedded tests
add_library(${BENCODE_LIBRARY_NAME}_Embedded ${BENCODE_EMBEDDED_SOURCES} ${BENCODE_INCLUDES})
add_library(Bencode_Lib::Bencode_Lib_Embedded ALIAS ${BENCODE_LIBRARY_NAME}_Embedded)

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

target_include_directories(${BENCODE_LIBRARY_NAME}_Embedded
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

if(BENCODE_BUILD_MINIMAL)
  target_include_directories(${BENCODE_LIBRARY_NAME}_Minimal
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
endif()

# Target-scoped warnings (replaces global add_compile_options)
if(MSVC)
  target_compile_options(${BENCODE_LIBRARY_NAME} PRIVATE
    /W4
    /permissive-
    /Zc:__cplusplus
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
  )

  if(BENCODE_BUILD_MINIMAL)
    target_compile_options(${BENCODE_LIBRARY_NAME}_Minimal PRIVATE
      /W4
      /permissive-
      /Zc:__cplusplus
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
    )
  endif()
else()
  target_compile_options(${BENCODE_LIBRARY_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
  )

  if(BENCODE_BUILD_MINIMAL)
    target_compile_options(${BENCODE_LIBRARY_NAME}_Minimal PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
    )
  endif()

  target_compile_options(${BENCODE_LIBRARY_NAME}_Embedded PRIVATE
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

target_precompile_headers(${BENCODE_LIBRARY_NAME}_Embedded
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h
)

if(BENCODE_BUILD_MINIMAL)
  target_precompile_headers(${BENCODE_LIBRARY_NAME}_Minimal
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h
  )
endif()

target_compile_definitions(${BENCODE_LIBRARY_NAME}
  PUBLIC
  BENCODE_EMBEDDED_MODE=$<IF:$<BOOL:${BENCODE_EMBEDDED_MODE}>,1,0>
  BENCODE_ENABLE_EXCEPTIONS=$<IF:$<BOOL:${BENCODE_ENABLE_EXCEPTIONS}>,1,0>
  BENCODE_ENABLE_FILE_IO=$<IF:$<BOOL:${BENCODE_ENABLE_FILE_IO}>,1,0>
  BENCODE_ENABLE_DYNAMIC_ALLOCATION=$<IF:$<BOOL:${BENCODE_ENABLE_DYNAMIC_ALLOCATION}>,1,0>
  BENCODE_ENABLE_JSON_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_JSON_STRINGIFY}>,1,0>
  BENCODE_ENABLE_XML_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_XML_STRINGIFY}>,1,0>
  BENCODE_ENABLE_YAML_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_YAML_STRINGIFY}>,1,0>
  BENCODE_MAX_NODE_COUNT=${BENCODE_MAX_NODE_COUNT}
  BENCODE_MAX_CONTAINER_SIZE=${BENCODE_MAX_CONTAINER_SIZE}
  BENCODE_MAX_STRING_LENGTH=${BENCODE_MAX_STRING_LENGTH}
)

target_compile_definitions(${BENCODE_LIBRARY_NAME}_Embedded
  PUBLIC
  BENCODE_EMBEDDED_MODE=1
  BENCODE_ENABLE_EXCEPTIONS=0
  BENCODE_ENABLE_FILE_IO=0
  BENCODE_ENABLE_DYNAMIC_ALLOCATION=0
  BENCODE_ENABLE_JSON_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_JSON_STRINGIFY}>,1,0>
  BENCODE_ENABLE_XML_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_XML_STRINGIFY}>,1,0>
  BENCODE_ENABLE_YAML_STRINGIFY=$<IF:$<BOOL:${BENCODE_ENABLE_YAML_STRINGIFY}>,1,0>
  BENCODE_MAX_NODE_COUNT=${BENCODE_MAX_NODE_COUNT}
  BENCODE_MAX_CONTAINER_SIZE=${BENCODE_MAX_CONTAINER_SIZE}
  BENCODE_MAX_STRING_LENGTH=${BENCODE_MAX_STRING_LENGTH}
)

if(BENCODE_BUILD_MINIMAL)
  target_compile_definitions(${BENCODE_LIBRARY_NAME}_Minimal
    PUBLIC
    BENCODE_EMBEDDED_MODE=$<IF:$<BOOL:${BENCODE_EMBEDDED_MODE}>,1,0>
    BENCODE_ENABLE_EXCEPTIONS=$<IF:$<BOOL:${BENCODE_ENABLE_EXCEPTIONS}>,1,0>
    BENCODE_ENABLE_FILE_IO=0
    BENCODE_ENABLE_DYNAMIC_ALLOCATION=$<IF:$<BOOL:${BENCODE_ENABLE_DYNAMIC_ALLOCATION}>,1,0>
    BENCODE_ENABLE_JSON_STRINGIFY=0
    BENCODE_ENABLE_XML_STRINGIFY=0
    BENCODE_ENABLE_YAML_STRINGIFY=0
    BENCODE_MAX_NODE_COUNT=${BENCODE_MAX_NODE_COUNT}
    BENCODE_MAX_CONTAINER_SIZE=${BENCODE_MAX_CONTAINER_SIZE}
    BENCODE_MAX_STRING_LENGTH=${BENCODE_MAX_STRING_LENGTH}
  )
endif()

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
