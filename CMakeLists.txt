project(KiteSTL)

####################
# include
####################
include_directories(${CMAKE_SOURCE_DIR}/inc)

####################
# source
####################
add_library(vector src/vector.c)

####################
# test
####################
enable_testing()

# test vector
add_executable(vector_test test/vector_test.c)
target_link_libraries(vector_test PRIVATE vector)
add_test(NAME vector_test COMMAND vector_test)