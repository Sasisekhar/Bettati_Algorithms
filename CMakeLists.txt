cmake_minimum_required(VERSION 3.16)

project(e2e_deadlines)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(e2e_deadlines main.cpp)

target_include_directories(e2e_deadlines PRIVATE ".")