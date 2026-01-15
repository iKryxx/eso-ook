cmake_minimum_required(VERSION 3.26)
project(Ook C)
set(PROJECT_VERSION 1.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Collect source and header files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/inc/*.h")


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ikook")

