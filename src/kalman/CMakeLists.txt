project(test_kalman)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


find_package(OpenCV)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

include_directories(./)
#add_executable(test_kalman main.cpp)
#add_executable(test_kalman kalman_mouse.cpp)
add_executable(test_kalman Kalman2.cpp)
target_link_libraries(test_kalman ${OpenCV_LIBRARIES})
