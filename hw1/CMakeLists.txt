cmake_minimum_required(VERSION 3.10)
project(hw1)

set(CMAKE_CXX_STANDART 11) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_subdirectory(lib)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Process)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)



