project(test_enum)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")


include_directories(./)
include_directories(./spdlog-1.9.2/include)
add_executable(test_enum main.cpp PerceptionRecorder.cpp)
target_link_libraries(test_enum pthread)
