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

set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall  -std=c++14")

# print_all_variables ()

# directories
set (source_dir ${CMAKE_SOURCE_DIR}/src)

set (client_dir ${source_dir}/client)
set (server_dir ${source_dir}/server)
set (common_dir ${source_dir}/common)

# files
set (common_files
     ${common_dir}/common.cpp)

set (udp_client_files 
     ${client_dir}/udp_client.cpp)
set (udp_server_files
     ${server_dir}/udp_server.cpp)


set (tcp_client_files 
     ${client_dir}/tcp_client.cpp)
set (tcp_server_files
     ${server_dir}/tcp_server.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include ${common_dir})

# print_all_variables()
add_executable(udp_client ${udp_client_files} ${common_files})
add_executable(udp_server ${udp_server_files} ${common_files})
add_executable(tcp_client ${tcp_client_files} ${common_files})
add_executable(tcp_server ${tcp_server_files} ${common_files})
