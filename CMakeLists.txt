cmake_minimum_required(VERSION 3.22)
project(VUT_FIT_PRL_proj2)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)

add_executable(VUT_FIT_PRL_proj2 preorder.cpp)

target_link_libraries(VUT_FIT_PRL_proj2 PUBLIC MPI::MPI_CXX)