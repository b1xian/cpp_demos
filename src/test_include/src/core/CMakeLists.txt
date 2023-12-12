project(vis_lib)

file(GLOB src_list *.cpp *.cc)

add_library(${PROJECT_NAME} SHARED
        ${src_list}
)
