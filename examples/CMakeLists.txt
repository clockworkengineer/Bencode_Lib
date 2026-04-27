cmake_minimum_required(VERSION 3.18.1)

project("Bencode_Lib Examples" VERSION 1.3.0 DESCRIPTION "Bencode_Lib example programs")

function(process_file target)
  add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/source/${target})
  target_include_directories(${target} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/external)

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

# Build each program
foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  process_file(${EXAMPLE_PROGRAM})
endforeach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})

add_custom_command(TARGET Bencode_Analyze_File.cpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)
