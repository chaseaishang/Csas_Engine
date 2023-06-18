cmake_minimum_required(VERSION 3.25)

project(CSAS_Renderer)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Renderer)
aux_source_directory(. RENDERER_DIR_LIB_SRCS)

target_include_directories(CSAS_Renderer
        INTERFACE
        .
        PRIVATE
        .
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Renderer PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Renderer
        PRIVATE
        ${RENDERER_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Renderer

        INTERFACE
        glfw glad::glad
        CSAS_Platform_OpenGL
        )



