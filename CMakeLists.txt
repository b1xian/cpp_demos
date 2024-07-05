cmake_minimum_required(VERSION 3.10)
set(demo_name cpp11_demo)
project(${demo_name})

set(CMAKE_CXX_STANDARD 17)
set(THIRD_PARTY_DIR ./third_party)

#--------------------------gtest--------------------------#
set(GTEST_VERSION 1.10.0)
set(GTEST_DIR ${THIRD_PARTY_DIR}/gtest/${GTEST_VERSION})
set(GTEST_INCLUDE_DIR ${GTEST_DIR}/include)
set(GTEST_LINK_DIR ${GTEST_DIR}/lib/${TARGET_OS}-${TARGET_ARCH})
set(GTEST_LIB gtest)
message(STATUS "[Dependency] gtest lib_dir = ${GTEST_LINK_DIR}")
#---------------------------------------------------------#

include(cmake/config.cmake)
add_subdirectory(src/observer)
add_subdirectory(src/visitor)
add_subdirectory(src/thread_pool)
add_subdirectory(src/test_condition)
add_subdirectory(src/test_bind)
add_subdirectory(src/cvtest)
add_subdirectory(src/socket)
add_subdirectory(src/time_test)
add_subdirectory(src/byte_test)
add_subdirectory(src/operator)
add_subdirectory(src/static_init)
add_subdirectory(src/namespace_link)
add_subdirectory(src/test_array_stack)
add_subdirectory(src/test_spd_log)
add_subdirectory(src/test_glog)
add_subdirectory(src/test_thread-exit)
add_subdirectory(src/test_2dimarray)
add_subdirectory(src/test_valgrind)
add_subdirectory(src/test_enum)
add_subdirectory(src/test_thread)
add_subdirectory(src/kalman)
add_subdirectory(src/test_ipm)
add_subdirectory(src/test_toml)
add_subdirectory(src/test_omp)
add_subdirectory(src/3_rongqi)
add_subdirectory(src/test_egine)
add_subdirectory(src/test_polygon)
add_subdirectory(src/test-tkcv)
add_subdirectory(src/ipc)
add_subdirectory(src/2023_interview)
add_subdirectory(src/test_opencv)

#if (${IS_COMPRESSED_IMAGE_TYPE} MATCHES true)
#    add_definitions(-DCOMPRESSED_IMAGE_TYPE)
#endif ()


add_executable(${demo_name} main.cpp
                include/TopicModule.h
                src/1_auto_decltype/auto.cpp
                src/1_auto_decltype/decltype.cpp
                src/4_thread/c_v_test.cpp
                src/4_thread/test_feature.cpp
                src/4_thread/atomic_test.cpp
                )
target_include_directories(${demo_name}
                            PUBLIC
                            ${GTEST_INCLUDE_DIR}
                            ${LINK_INCLUDE_DIR}
        )
target_link_directories(${demo_name}
                        PUBLIC
                        ${GTEST_LINK_DIR})
target_link_libraries(${demo_name}
                        PUBLIC
                        ${GTEST_LIB}
                        pthread)
target_compile_options(${demo_name} PRIVATE -std=c++11)
#target_compile_options(${LIB_NAME} PRIVATE -Ofast -ffast-math -Wall -Wextra -fPIC -frtti -fexceptions -Wno-unused-parameter -Wno-format-security -Wno-unused-variable)


