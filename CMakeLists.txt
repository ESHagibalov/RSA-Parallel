cmake_minimum_required(VERSION 3.20)
project(RSA)

set(CMAKE_CXX_STANDARD 14)

add_executable(RSA main.cpp)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(RSA PUBLIC OpenMP::OpenMP_CXX)
endif ()