cmake_minimum_required(VERSION 3.25)

project(CSAS_Platform_OpenGL)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Platform_OpenGL)

aux_source_directory(. PLATFORM_OPENGL_DIR_LIB_SRCS)

target_include_directories(CSAS_Platform_OpenGL
        INTERFACE
        ./

        PRIVATE
        ./
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        ${CSAS_ENGINE_VENDOR_PATH}
        )
target_precompile_headers(CSAS_Platform_OpenGL PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Platform_OpenGL
        PRIVATE
        ${PLATFORM_OPENGL_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Platform_OpenGL

        INTERFACE
        glfw glad::glad
        Stb_image
        )