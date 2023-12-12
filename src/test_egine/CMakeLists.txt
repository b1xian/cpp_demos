project(test_egine)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


find_package(PkgConfig REQUIRED)
################################################################

# Eigen3 (.pc)
pkg_check_modules(Eigen3 REQUIRED eigen3)
# pkg-config  --cflags --libs eigen3
################################################################


include_directories(
        ${Eigen3_INCLUDE_DIRS})

add_executable(test_egine main.cpp)
