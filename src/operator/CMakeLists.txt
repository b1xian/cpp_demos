project(operator_test)

#--------------------------yaml--------------------------#
set(YAML_VERSION 0.5.3)
set(YAML_DIR yaml/${YAML_VERSION})
set(YAML_INCLUDE_DIR ${YAML_DIR}/include)
set(YAML_LINK_DIR ${YAML_DIR}/lib/${TARGET_OS}-${TARGET_ARCH})
file (GLOB YAML_LIBS ${YAML_LINK_DIR}/*.so*)
message(STATUS "[Dependency] yaml lib_dir = ${YAML_LINK_DIR}")
message(STATUS "[Dependency] yaml libs = ${YAML_LIBS}")
#---------------------------------------------------------#

add_executable(operator_test
        main.cpp
        VisionConfig.cpp
)

include_directories(./)
include_directories(${YAML_INCLUDE_DIR})
target_link_libraries(operator_test ${YAML_LIBS})
