cmake_minimum_required(VERSION 3.16.0)

project("BencodeLib" VERSION 0.1.0 DESCRIPTION "Bencode C++ Static Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

if(MSVC)
  add_compile_options(/W4 /D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Werror -pedantic)
endif()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

# Enable unit tests

enable_testing()

configure_file(BencodeConfig.h.in BencodeConfig.hpp)

set(BENCODE_LIBRARY_NAME "${PROJECT_NAME}")

# Enable unit tests
 
set (BENCODE_SOURCES
    ./classes/Bencode.cpp
)

set (BENCODE_INCLUDES
    ./include/Bencode.hpp
    ./include/BNode.hpp
    ./include/Bencoding.hpp
    ./include/BencodeSources.hpp
    ./include/BencodeDestinations.hpp
)

# Bencode library

add_library(${BENCODE_LIBRARY_NAME} STATIC ${BENCODE_SOURCES})
target_include_directories(${BENCODE_LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR})

# Add tests

add_subdirectory(tests)

# install

install(TARGETS ${BENCODE_LIBRARY_NAME} DESTINATION lib)
install(FILES ${BENCODE_INCLUDES} DESTINATION include)


