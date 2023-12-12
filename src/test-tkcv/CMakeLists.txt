project(test_tk_cv)

set(tkcv_name test_tk_cv)

# opencv
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86_64)
    find_package(OpenCV REQUIRED)
else()
    find_package(OpenCV 4 REQUIRED)
endif()
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "Not found OpenCV, you should install ROS or cross-compile opencv")
endif()
include_directories(
        ${CMAKE_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)


add_executable(${tkcv_name} main.cpp)
target_link_libraries(${tkcv_name}
        ${OpenCV_LIBRARIES}
)
