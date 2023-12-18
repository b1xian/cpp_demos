project(test_interview)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

message("-- CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(GTEST_VERSION 1.10.0)
set(GTEST_DIR ${THIRD_PARTY_DIR}/gtest/${GTEST_VERSION})
set(GTEST_INCLUDE_DIR ${GTEST_DIR}/include)
set(GTEST_LINK_DIR ${GTEST_DIR}/lib/linux-x86_64)
message("-- [Dependency] gtest lib_dir = ${GTEST_LINK_DIR}")
set(GTEST_LIB gtest)

find_package(OpenCV 3)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
)


file(GLOB src_list  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(./)
#add_executable(test_kalman main.cpp)
#add_executable(test_kalman kalman_mouse.cpp)
add_executable(test_interview ${src_list})
target_link_directories(test_interview PUBLIC
        ${GTEST_LINK_DIR})
target_link_libraries(test_interview
        ${OpenCV_LIBRARIES}
        ${GTEST_LIB}
        pthread
)
