cmake_minimum_required (VERSION 3.22)

project (socktools VERSION 1.0.0)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14)

# directories
set (source_dir ${CMAKE_SOURCE_DIR}/src)

set (client_dir ${source_dir}/client)
set (server_dir ${source_dir}/server)
set (common_dir ${source_dir}/common)

# files
set (common_files
     ${common_dir}/common.c)

set (client_files 
     ${client_dir}/client.c)

set (server_files
     ${server_dir}/server.c)


include_directories(${CMAKE_SOURCE_DIR}/include ${common_dir})

# print_all_variables()
add_executable(client ${client_files} ${common_files})
add_executable(server ${server_files} ${common_files})
