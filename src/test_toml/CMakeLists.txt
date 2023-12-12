set(lib_name test_toml)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${lib_name} main.cpp)