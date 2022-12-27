# cmake_minimum_required(VERSION 3.15)
# project(myMuduo)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -fPIC")

aux_source_directory(. SRC)
add_library(myMuduo SHARED ${SRC})