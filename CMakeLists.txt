cmake_minimum_required(VERSION 3.15)
project(httpServer)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src/muduo)
include_directories(${PROJECT_SOURCE_DIR}/src/http_server)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src/muduo)
add_subdirectory(src/http_server)


