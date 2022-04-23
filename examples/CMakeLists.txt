cmake_minimum_required(VERSION 3.16.0)

project("Bencode Examples" VERSION 0.1.0 DESCRIPTION "BencodeLib example programs")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

if(MSVC)
  add_compile_options(/W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

# Get example program list

file( GLOB EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

# Build each program

foreach( EXAMPLE_PROGRAM ${EXAMPLE_SOURCES} )
    string( REPLACE ".cpp" "" EXAMPLE_TARGET ${EXAMPLE_PROGRAM} )
    add_executable( ${EXAMPLE_TARGET} ${EXAMPLE_PROGRAM} )
    target_link_libraries( ${EXAMPLE_TARGET} BencodeLib )
    install(TARGETS ${EXAMPLE_TARGET} DESTINATION bin)
endforeach( EXAMPLE_PROGRAM ${EXAMPLE_SOURCES} )

message(${CMAKE_CURRENT_BINARY_DIR}"/"$<CONFIG>/)