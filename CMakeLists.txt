cmake_minimum_required(VERSION 3.16.3)

project("Bencode_Lib" VERSION 0.1.0 DESCRIPTION "Bencode C++ Library" LANGUAGES CXX)

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
 
set (BENCODE_SOURCES
    ./classes/Bencode.cpp
    ./classes/implementation/Bencode_BNode.cpp
    ./classes/implementation/Bencode_Impl.cpp)

set (BENCODE_INCLUDES
    ./include/Bencode.hpp
    ./include/interface/ISource.hpp
    ./include/interface/IDestination.hpp
    ./include/implementation/Bencode_Types.hpp
    ./include/implementation/Bencode_Impl.hpp
    ./include/implementation/Bencode_Sources.hpp
    ./include/implementation/Bencode_Destinations.hpp)

# Bencode library

add_library(${BENCODE_LIBRARY_NAME} ${BENCODE_SOURCES} ${BENCODE_INCLUDES})
target_include_directories(${BENCODE_LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/interface ${PROJECT_SOURCE_DIR}/include/implementation ${PROJECT_SOURCE_DIR}/include/external ${PROJECT_BINARY_DIR})

# Add tests

add_subdirectory(tests)

# Add examples

add_subdirectory(examples)

# install

install(TARGETS ${BENCODE_LIBRARY_NAME} DESTINATION lib)
install(FILES ${BENCODE_INCLUDES} DESTINATION include)


