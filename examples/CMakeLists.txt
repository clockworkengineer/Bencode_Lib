cmake_minimum_required(VERSION 3.16.3)

project("Bencode_Lib Examples" VERSION 1.1.0 DESCRIPTION "Bencode_Lib example programs")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

# Get example program list
file(GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/ "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# Build each program
foreach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})
  string(REPLACE ".cpp" "" EXAMPLE_TARGET ${EXAMPLE_PROGRAM})
  add_executable(${EXAMPLE_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/source/${EXAMPLE_PROGRAM})
  target_include_directories(${EXAMPLE_TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include  ${PROJECT_SOURCE_DIR}/include/external)
  target_link_libraries(${EXAMPLE_TARGET} Bencode_Lib)
  install(TARGETS ${EXAMPLE_TARGET} DESTINATION bin)
endforeach(EXAMPLE_PROGRAM ${EXAMPLE_SOURCES})

add_custom_command(TARGET Bencode_Read_Torrent_Files POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/files ${PROJECT_BINARY_DIR}/files)
