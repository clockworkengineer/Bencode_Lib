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
    Bencode_Optional_Stringify.hpp
)

foreach(header IN LISTS REQUIRED_PUBLIC_HEADERS)
    if(NOT EXISTS "${INCLUDEDIR}/${header}")
        message(FATAL_ERROR "Required public header missing from install tree: ${header}")
    endif()
endforeach()

set(ALLOWED_IMPLEMENTATION_HEADERS
    implementation/common/Bencode_Error.hpp
    implementation/node/Bencode_Node.hpp
    implementation/node/Bencode_Node_Creation.hpp
    implementation/node/Bencode_Node_Index.hpp
    implementation/node/Bencode_Node_Reference.hpp
    implementation/variants/Bencode_Variant.hpp
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
    implementation/translator/Default_Translator.hpp
    implementation/stringify/JSON_Stringify.hpp
    implementation/stringify/Default_Translator.hpp
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

message(STATUS "Package contents validated successfully.")
