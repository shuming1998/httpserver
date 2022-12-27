cmake_minimum_required(VERSION 3.15)
project(httpServer)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/muduo)
include_directories(${PROJECT_SOURCE_DIR}/http_server)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(muduo)
add_subdirectory(http_server)


