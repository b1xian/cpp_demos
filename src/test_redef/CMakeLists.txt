project(test_enum)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


include_directories(./)
add_executable(test_redef main.cpp)
