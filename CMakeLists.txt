cmake_minimum_required(VERSION 3.16)
project(llvm-kaleidoscope)

find_package(LLVM REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE kaleidoscope_SRCS ${PROJECT_SOURCE_DIR} *.cpp *.h)

add_executable(${PROJECT_NAME} ${kaleidoscope_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(${PROJECT_NAME} ${llvm_libs})