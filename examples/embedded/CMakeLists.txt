cmake_minimum_required(VERSION 3.21)

project("Bencode_Lib Embedded Example" VERSION 1.3.0 DESCRIPTION "Bencode_Lib Embedded Configuration Example" LANGUAGES CXX)

# Embedded mode settings
set(BENCODE_EMBEDDED_MODE ON CACHE BOOL "Enable embedded build mode")
set(BENCODE_ENABLE_EXCEPTIONS OFF CACHE BOOL "Disable exception support")
set(BENCODE_ENABLE_FILE_IO OFF CACHE BOOL "Disable file-based I/O support")
set(BENCODE_ENABLE_DYNAMIC_ALLOCATION OFF CACHE BOOL "Disable dynamic allocation for containers")
set(BENCODE_MAX_NODE_COUNT 128 CACHE STRING "Maximum total node count in embedded mode")
set(BENCODE_MAX_CONTAINER_SIZE 32 CACHE STRING "Maximum number of elements in embedded lists/dictionaries")
set(BENCODE_MAX_STRING_LENGTH 1024 CACHE STRING "Maximum string length in embedded mode")

# Add the library root as a subdirectory. Adjust the path as needed for your workspace layout.
add_subdirectory(../.. bencode-lib)

add_executable(Embedded_Example source/Embedded_Example.cpp)
add_compile_definitions(BENCODE_EMBEDDED_MODE)
target_link_libraries(Embedded_Example PRIVATE Bencode_Lib::Bencode_Lib_Embedded)
