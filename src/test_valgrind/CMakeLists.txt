project(test_valgrind)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")



add_executable(test_valgrind main.cpp)
target_link_libraries(test_valgrind
        pthread
)