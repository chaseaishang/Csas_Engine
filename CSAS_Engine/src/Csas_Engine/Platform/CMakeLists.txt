cmake_minimum_required(VERSION 3.25)

project(CSAS_Platform)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Linux/)
add_subdirectory(OpenGL/)

# 生成链接库

add_library (CSAS_Platform INTERFACE)

target_include_directories(CSAS_Platform
        INTERFACE
        Linux
        ${CSAS_ENGINE_VENDOR_PATH}
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Platform INTERFACE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_link_libraries(CSAS_Platform

        INTERFACE
        CSAS_Core
        glfw glad::glad
        CSAS_Platform_OpenGL
        CSAS_Platform_Linux
        )
#CSAS_Platform_OpenGL