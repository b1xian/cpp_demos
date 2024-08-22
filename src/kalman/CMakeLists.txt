project(test_kalman)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


find_package(OpenCV)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

find_package(PkgConfig REQUIRED)
################################################################
# Eigen3 (.pc)
pkg_check_modules(Eigen3 REQUIRED eigen3)
# pkg-config  --cflags --libs eigen3
################################################################


include_directories(
        ${Eigen3_INCLUDE_DIRS})
include_directories(./)
#add_executable(test_kalman main.cpp)
#add_executable(test_kalman kalman_mouse.cpp)
add_executable(test_kalman Kalman2.cpp)
target_link_libraries(test_kalman ${OpenCV_LIBRARIES})

add_executable(test_kalman_rollback Kalman_rollback.cpp)
target_link_libraries(test_kalman_rollback ${OpenCV_LIBRARIES})

add_executable(test_kalman_ctra Kalman_CTRA.cpp)
target_link_libraries(test_kalman_ctra ${OpenCV_LIBRARIES})

add_executable(test_kalman_angle KalmanAngle.cpp)
target_link_libraries(test_kalman_angle ${OpenCV_LIBRARIES})
