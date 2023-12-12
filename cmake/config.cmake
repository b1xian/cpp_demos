file(READ "configs/TopicModule.yml" yam_config)
string(REGEX MATCH "isCompressedImageType: ([a-zA-Z]*)" _ ${yam_config})
set(IS_COMPRESSED_IMAGE_TYPE ${CMAKE_MATCH_1})
message(${IS_COMPRESSED_IMAGE_TYPE})
configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/include/build_config.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/build_config.h"
)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/build_config.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/)

