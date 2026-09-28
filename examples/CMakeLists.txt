cmake_minimum_required(VERSION 3.18.1)

project("Bencode_Lib Examples" VERSION 1.3.0 DESCRIPTION "Bencode_Lib example programs")

if(BENCODE_EMBEDDED_MODE)
  return()
endif()

function(process_file target)
  add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/source/${target})
  target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/classes/include
    ${CMAKE_SOURCE_DIR}/classes/include/implementation
    ${CMAKE_SOURCE_DIR}/classes/include/implementation/stringify
  )

  if(BENCODE_BUILD_MINIMAL AND target STREQUAL "Bencode_Minimal_Encode_Decode.cpp")
    target_link_libraries(${target} Bencode_Lib::Bencode_Lib_Minimal)
    target_precompile_headers(${target} REUSE_FROM Bencode_Lib_Minimal)
  else()
    target_link_libraries(${target} Bencode_Lib::Bencode_Lib)
    target_precompile_headers(${target} REUSE_FROM Bencode_Lib)
  endif()

  if(MSVC)
    target_compile_options(${target} PRIVATE
      /W4
      /permissive-
      /Zc:__cplusplus
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:/WX>
    )
  else()
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      $<$<BOOL:${BENCODE_WARNINGS_AS_ERRORS}>:-Werror>
    )
  endif()

  install(TARGETS ${target} DESTINATION bin)
endfunction()

# Get example program list
file(GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/ "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
list(REMOVE_ITEM EXAMPLE_SOURCES "Bencode_Modules_Example.cpp")

# Copy example input files into the example build directory so runtime examples can find them.
add_custom_target(copy_example_files ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/files
  ${CMAKE_CURRENT_BINARY_DIR}/files
  COMMENT "Copy example files into build/examples/files"
)

# Build each program
foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  process_file(${EXAMPLE_PROGRAM})
  add_dependencies(${EXAMPLE_PROGRAM} copy_example_files)
endforeach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})

# C++23 Named Modules Example Target
find_program(CLANG_COMPILER_BIN NAMES clang++-18 clang++-17 clang++)
if(CLANG_COMPILER_BIN AND NOT BENCODE_EMBEDDED_MODE)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Bencode_Modules_Example
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/modules_build
    COMMAND ${CLANG_COMPILER_BIN} -std=c++23
      -DBENCODE_ENABLE_EXCEPTIONS=1 -DBENCODE_ENABLE_FILE_IO=1 -DBENCODE_ENABLE_DYNAMIC_ALLOCATION=1
      -I${CMAKE_BINARY_DIR}
      -I${CMAKE_SOURCE_DIR}/classes/include
      -I${CMAKE_SOURCE_DIR}/classes/include/implementation
      -I${CMAKE_SOURCE_DIR}/classes/include/implementation/stringify
      --precompile -x c++-module ${CMAKE_SOURCE_DIR}/classes/modules/Bencode_Lib.cppm
      -o ${CMAKE_CURRENT_BINARY_DIR}/modules_build/Bencode_Lib.pcm
    COMMAND ${CLANG_COMPILER_BIN} -std=c++23 -c
      ${CMAKE_CURRENT_BINARY_DIR}/modules_build/Bencode_Lib.pcm
      -o ${CMAKE_CURRENT_BINARY_DIR}/modules_build/Bencode_Lib.o
    COMMAND ${CLANG_COMPILER_BIN} -std=c++23
      -fprebuilt-module-path=${CMAKE_CURRENT_BINARY_DIR}/modules_build
      ${CMAKE_CURRENT_SOURCE_DIR}/source/Bencode_Modules_Example.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/modules_build/Bencode_Lib.o
      $<TARGET_FILE:Bencode_Lib::Bencode_Lib>
      -o ${CMAKE_CURRENT_BINARY_DIR}/Bencode_Modules_Example
    DEPENDS ${BENCODE_LIBRARY_NAME}
            ${CMAKE_SOURCE_DIR}/classes/modules/Bencode_Lib.cppm
            ${CMAKE_CURRENT_SOURCE_DIR}/source/Bencode_Modules_Example.cpp
    COMMENT "Building C++23 Named Module Example (Bencode_Modules_Example)"
  )
  add_custom_target(Bencode_Lib_Modules_Example ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Bencode_Modules_Example
  )
  add_test(NAME Bencode_Lib_Modules_Example_Run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Bencode_Modules_Example
  )
endif()
