cmake_minimum_required(VERSION 3.25)

project(CSAS_Platform_Linux)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)



# 生成链接库

add_library (CSAS_Platform_Linux)

aux_source_directory(. PLATFORM_LINUX_DIR_LIB_SRCS)

target_include_directories(CSAS_Platform_Linux
        INTERFACE
        .
        PRIVATE
        .
        ${CSAS_ENGINE_VENDOR_PATH}
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Platform_Linux PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Platform_Linux
        PRIVATE
        ${PLATFORM_LINUX_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Platform_Linux
        PRIVATE
        CSAS_Core
        glfw glad::glad

        )
#CSAS_Platform_OpenGL