cmake_minimum_required(VERSION 3.10)
project(test_glog)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")
add_definitions(-DGLOG)

set(glog_version 1.9.2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glog-${glog_version}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glog-${glog_version}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB src_list *.cpp)

add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME}
    glog
    pthread
)


