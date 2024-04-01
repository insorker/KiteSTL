cmake_minimum_required(VERSION 2.8.12)

project(KiteSTL)

add_compile_options(
  "-O3"
  "$<$<CONFIG:DEBUG>:-Wall;-Werror;-fsanitize=address;-O0;-g;-ggdb>"
)
add_link_options("$<$<CONFIG:DEBUG>:-fsanitize=address>")

####################
# include
####################
include_directories(${CMAKE_SOURCE_DIR}/inc)

####################
# source
####################

add_library(cemu STATIC src/cemu.c)
add_library(str STATIC src/str.c)
# add_library(map src/map.c)
# add_library(treap src/treap.c)
add_library(vector STATIC src/vector.c)

####################
# lib
####################
# file(GLOB CEMU_SRC ${CMAKE_SOURCE_DIR}/src/cemu/*.c)
# file(GLOB STL_SRC ${CMAKE_SOURCE_DIR}/src/*.c)
# set(LIB_SRC ${CEMU_SRC} ${STL_SRC})

# ADD_LIBRARY(KiteSTL SHARED ${LIB_SRC})
add_library(KiteSTL SHARED src/vector.c)

####################
# test
####################
enable_testing()

add_executable(test_str test/test_str.c)
target_link_libraries(test_str PRIVATE str cemu)
add_test(NAME test_str COMMAND test_str)

add_executable(test_vector test/test_vector.c)
target_link_libraries(test_vector PRIVATE vector str cemu)
add_test(NAME test_vector COMMAND test_vector)

# add_executable(test_map test/test_map.c)
# target_link_libraries(test_map PRIVATE map treap vector str cemu)
# add_test(NAME test_map COMMAND test_map)