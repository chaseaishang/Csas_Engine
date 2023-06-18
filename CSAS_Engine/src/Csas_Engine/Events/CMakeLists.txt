cmake_minimum_required(VERSION 3.25)

project(CSAS_Events)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Events)
aux_source_directory(src/ EVENT_DIR_LIB_SRCS)
message(STATUS 666${EVENT_DIR_LIB_SRCS})
target_include_directories(CSAS_Events
        INTERFACE
        include
        PRIVATE
        include
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Events PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Events
        PRIVATE
        ${EVENT_DIR_LIB_SRCS}
        )




