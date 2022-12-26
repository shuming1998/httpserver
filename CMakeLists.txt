cmake_minimum_required(VERSION 3.15)
project(httpServer)

# set(CMAKE_CXX_FLAGS "$ {CMAKE_CXX_FLAGS} -fPIC")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/http_server)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/muduo)
include_directories(${PROJECT_SOURCE_DIR}/http_server)
# link_directories(${PROJECT_SOURCE_DIR}/muduo/lib)

add_subdirectory(muduo)
# add_subdirectory(http_server)


