cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "-Ofast")

# project name
project(voxel)

set(LIBRARIES glfw GL GLEW)

include_directories(includes)
include_directories(vendor)

file(GLOB SOURCES src/*.cpp)
file(GLOB VENDOR vendor/**/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${VENDOR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBRARIES})