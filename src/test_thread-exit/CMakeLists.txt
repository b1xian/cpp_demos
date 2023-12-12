project(test_thread_exit)


add_executable(test_thread_exit main.cpp)
target_link_libraries(test_thread_exit
    pthread
)