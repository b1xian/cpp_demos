project(test_cv)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#add_executable(test_cv testcv.cpp)
add_executable(test_cv test_kalman_2.cpp)


target_link_libraries(test_cv
        ${OpenCV_LIBRARIES})