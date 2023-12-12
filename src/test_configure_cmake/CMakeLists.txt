cmake_minimum_required(VERSION 3.10.2)

set(project_name TestCondLaneNet)
project(${project_name})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

include(CMakePackageConfigHelpers)
set(LIB_INSTALL_DIR ./)

set(CURB_MODEL_INSTALL_DIR model/CondLaneNet/model/ )
set(CURB_MODEL_CONFIG_FILE model/CondLaneNet/conf/config.toml)
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/curb_model-config.cmake.in
                                ${CMAKE_CURRENT_BINARY_DIR}/cmake/curb-model/curb_model-config.cmake
                                INSTALL_DESTINATION ${LIB_INSTALL_DIR}/share/cmake/curb-model
                                PATH_VARS CURB_MODEL_INSTALL_DIR CURB_MODEL_CONFIG_FILE)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/curb-model/curb_model-config-version.cmake
                                VERSION 2.0.0
                                COMPATIBILITY SameMajorVersion)



set(SDKCondLaneNet_INCLUDE_DIRS include)
set(SDKCondLaneNet_LIBRARY_DIRS lib)
set(SDKCondLaneNet_INSTALL_PATH ./)
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/sdkcondlanenet-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/sdkcondlanenet/sdkcondlanenet-config.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/share/cmake/SDKCondLaneNet
        PATH_VARS SDKCondLaneNet_INCLUDE_DIRS SDKCondLaneNet_LIBRARY_DIRS SDKCondLaneNet_INSTALL_PATH)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/sdkcondlanenet/sdkcondlanenet-config-version.cmake
        VERSION 2.0.0
        COMPATIBILITY SameMajorVersion)


set(INCLUDE_INSTALL_DIR include/)
set(SYSCONFIG_INSTALL_DIR etc/foo/)
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/FooConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/Foo/FooConfig.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/share/cmake/Foo
        PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/Foo/FooConfigVersion.cmake
        VERSION 1.2.3
        COMPATIBILITY SameMajorVersion )