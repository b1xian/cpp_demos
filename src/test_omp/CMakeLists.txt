project(test_omp)

set(proj_name test_omp)

#add_definitions("-fopenmp")

find_package(OpenMP)

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


add_executable(${proj_name} main.cpp)
target_link_libraries(${proj_name}
        ${OpenCV_LIBRARIES}
        OpenMP::OpenMP_CXX
)
