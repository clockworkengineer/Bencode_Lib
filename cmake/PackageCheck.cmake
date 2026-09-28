cmake_minimum_required(VERSION 3.21)

set(INSTALL_PREFIX "")

# CMake script mode does not expose ARGV/ARGC like functions do. Use CMAKE_ARGC and CMAKE_ARGV<n>.
math(EXPR SCRIPT_ARG_START_INDEX 3)
math(EXPR SCRIPT_ARG_LAST_INDEX "${CMAKE_ARGC} - 1")

if(SCRIPT_ARG_LAST_INDEX GREATER_EQUAL SCRIPT_ARG_START_INDEX)
    foreach(arg_index RANGE ${SCRIPT_ARG_START_INDEX} ${SCRIPT_ARG_LAST_INDEX})
        set(arg "${CMAKE_ARGV${arg_index}}")
        if(arg STREQUAL "--")
            continue()
        endif()
        if(arg STREQUAL "--install-prefix")
            math(EXPR NEXT_INDEX "${arg_index} + 1")
            if(NEXT_INDEX GREATER SCRIPT_ARG_LAST_INDEX)
                message(FATAL_ERROR "Missing value for --install-prefix")
            endif()
            set(INSTALL_PREFIX "${CMAKE_ARGV${NEXT_INDEX}}")
            break()
        endif()
    endforeach()
endif()

if(NOT INSTALL_PREFIX)
    message(FATAL_ERROR "PackageCheck.cmake requires --install-prefix <path>")
endif()

set(INCLUDEDIR "${INSTALL_PREFIX}/include")

if(NOT EXISTS "${INCLUDEDIR}")
    message(FATAL_ERROR "Installed include directory not found: ${INCLUDEDIR}")
endif()

set(REQUIRED_PUBLIC_HEADERS
    Bencode.hpp
    Bencode_Core.hpp
    Bencode_Status.hpp
    Bencode_View.hpp
    Bencode_Serialization.hpp
    Bencode_Optional_Stringify.hpp
)

foreach(header IN LISTS REQUIRED_PUBLIC_HEADERS)
    if(NOT EXISTS "${INCLUDEDIR}/${header}")
        message(FATAL_ERROR "Required public header missing from install tree: ${header}")
    endif()
endforeach()

set(ALLOWED_IMPLEMENTATION_HEADERS
    implementation/common/Bencode_Error.hpp
    implementation/common/Bencode_Status.hpp
    implementation/common/Bencode_Parser_Constants.hpp
    implementation/node/Bencode_Node.hpp
    implementation/node/Bencode_Node_Creation.hpp
    implementation/node/Bencode_Node_Index.hpp
    implementation/node/Bencode_Node_Reference.hpp
    implementation/variants/Bencode_Variant.hpp
    implementation/variants/Bencode_FixedVector.hpp
    implementation/variants/Bencode_Hole.hpp
    implementation/variants/Bencode_Integer.hpp
    implementation/variants/Bencode_String.hpp
    implementation/variants/Bencode_Dictionary.hpp
    implementation/variants/Bencode_List.hpp
    implementation/io/Bencode_Sources.hpp
    implementation/io/Bencode_Destinations.hpp
    implementation/io/Bencode_BufferSource.hpp
    implementation/io/Bencode_BufferDestination.hpp
    implementation/io/Bencode_FileSource.hpp
    implementation/io/Bencode_FileDestination.hpp
    implementation/view/Bencode_NodeView.hpp
    implementation/translator/Default_Translator.hpp
    implementation/translator/XML_Translator.hpp
    implementation/stringify/JSON_Stringify.hpp
    implementation/stringify/XML_Stringify.hpp
    implementation/stringify/YAML_Stringify.hpp
)

file(GLOB_RECURSE INSTALLED_IMPLEMENTATION_HEADERS
    RELATIVE "${INCLUDEDIR}"
    "${INCLUDEDIR}/implementation/*.hpp"
    "${INCLUDEDIR}/implementation/*/*.hpp"
)

foreach(header IN LISTS INSTALLED_IMPLEMENTATION_HEADERS)
    list(FIND ALLOWED_IMPLEMENTATION_HEADERS ${header} found)

    if(found EQUAL -1)
        message(FATAL_ERROR "Unexpected implementation header installed: ${header}")
    endif()
endforeach()

message(STATUS "Validating downstream consumer compilation with find_package(Bencode_Lib)...")

set(TEST_DIR "${INSTALL_PREFIX}/test_consumer")
file(MAKE_DIRECTORY "${TEST_DIR}")

file(WRITE "${TEST_DIR}/CMakeLists.txt" [=[
cmake_minimum_required(VERSION 3.21)
project(PackageCheckConsumer CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Bencode_Lib REQUIRED CONFIG HINTS "${CMAKE_CURRENT_LIST_DIR}/..")

add_executable(consumer_app main.cpp)
target_link_libraries(consumer_app PRIVATE Bencode_Lib::Bencode_Lib)
]=])

file(WRITE "${TEST_DIR}/main.cpp" [=[
#include <Bencode.hpp>
#include <Bencode_Core.hpp>
#include <Bencode_Optional_Stringify.hpp>
#include <iostream>

int main() {
    Bencode_Lib::Bencode b;
    b.parse("d3:agei25e4:name4:Janee");
    if (b["name"].as_string().value_or("") != "Jane") {
        return 1;
    }
    Bencode_Lib::BufferDestination dest;
    Bencode_Lib::JSON_Stringify jsonStringify;
    jsonStringify.stringify(b.root(), dest);
    if (dest.toString().empty()) {
        return 2;
    }
    std::cout << "Downstream consumer execution successful: " << dest.toString() << std::endl;
    return 0;
}
]=])

execute_process(
    COMMAND "${CMAKE_COMMAND}" -B "${TEST_DIR}/build" -S "${TEST_DIR}"
    RESULT_VARIABLE config_res
    OUTPUT_VARIABLE config_out
    ERROR_VARIABLE config_err
)
if(NOT config_res EQUAL 0)
    message(FATAL_ERROR "Downstream consumer CMake configure failed:\n${config_out}\n${config_err}")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${TEST_DIR}/build"
    RESULT_VARIABLE build_res
    OUTPUT_VARIABLE build_out
    ERROR_VARIABLE build_err
)
if(NOT build_res EQUAL 0)
    message(FATAL_ERROR "Downstream consumer CMake build failed:\n${build_out}\n${build_err}")
endif()

execute_process(
    COMMAND "${TEST_DIR}/build/consumer_app"
    RESULT_VARIABLE run_res
    OUTPUT_VARIABLE run_out
    ERROR_VARIABLE run_err
)
if(NOT run_res EQUAL 0)
    message(FATAL_ERROR "Downstream consumer application run failed:\n${run_out}\n${run_err}")
endif()

file(REMOVE_RECURSE "${TEST_DIR}")

message(STATUS "Package contents and downstream consumer validated successfully.")
