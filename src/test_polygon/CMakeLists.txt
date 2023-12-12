project(test_polygon)



find_package(OpenCV REQUIRED)
link_libraries(${OpenCV_LIBRARIES})
message("-- found opencv:${OpenCV_VERSION}")

include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(test_polygon
        main.cpp
        )
target_link_libraries(test_polygon ${OpenCV_LIBRARIES})

