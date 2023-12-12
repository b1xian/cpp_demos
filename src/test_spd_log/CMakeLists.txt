cmake_minimum_required(VERSION 3.10)
project(test_spd)

add_definitions(-DSPDLOG)
#add_definitions(-DLOG_TO_CONSOLE)

set(spd_version 1.9.2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/spd-${spd_version}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/spd-${spd_version}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB src_list *.cpp)

add_executable(${PROJECT_NAME} ${src_list})
target_link_libraries(${PROJECT_NAME}
    pthread
)


