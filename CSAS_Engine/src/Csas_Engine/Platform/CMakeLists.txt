cmake_minimum_required(VERSION 3.25)

project(CSAS_Platform)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Platform)

aux_source_directory(Linux/ PLATFORM_DIR_LIB_SRCS)

target_include_directories(CSAS_Platform
        INTERFACE
        Linux
        PRIVATE
        Linux
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Platform PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Platform
        PRIVATE
        ${PLATFORM_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Platform

        INTERFACE
        glfw glad::glad
        )