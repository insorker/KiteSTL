cmake_minimum_required(VERSION 2.8.12)

project(KiteSTL)

add_compile_options(
  "-O3"
  "$<$<CONFIG:DEBUG>:-Wall;-Werror;-fsanitize=address;-O0;-g;-ggdb>"
)
add_link_options("-fsanitize=address")

####################
# include
####################
include_directories(${CMAKE_SOURCE_DIR}/inc)

####################
# source
####################
add_library(cemu src/cemu.c)
add_library(cemu_int src/cemu_int.c)
add_library(cemu_str src/cemu_str.c)
add_library(map src/map.c)
add_library(treap src/treap.c)
add_library(vector src/vector.c)

####################
# lib
####################
file(GLOB LIB_SRC ${CMAKE_SOURCE_DIR}/src/*.c)
ADD_LIBRARY(KiteSTL SHARED ${LIB_SRC})

####################
# test
####################
enable_testing()

add_executable(vector_test test/vector_test.c)
target_link_libraries(vector_test PRIVATE vector cemu_int cemu_str cemu)
add_test(NAME vector_test COMMAND vector_test)

add_executable(map_test test/map_test.c)
target_link_libraries(map_test PRIVATE map treap vector cemu_int cemu_str cemu)
add_test(NAME map_test COMMAND map_test)