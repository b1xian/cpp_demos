project(thread_pool)

add_executable(thread_pool
        main.cpp
        )
target_link_libraries(thread_pool pthread)

