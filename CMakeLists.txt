project(KiteSTL)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -Wall -Werror -g")

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
# test
####################
enable_testing()

# test vector
add_executable(vector_test test/vector_test.c)
target_link_libraries(vector_test PRIVATE vector cemu_int cemu_str cemu)
add_test(NAME vector_test COMMAND vector_test)

add_executable(map_test test/map_test.c)
target_link_libraries(map_test PRIVATE map treap vector cemu_int cemu_str cemu)
add_test(NAME map_test COMMAND map_test)