cmake_minimum_required(VERSION 3.25)

project(CSAS_Core)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Core)
aux_source_directory(. Core_DIR_LIB_SRCS)

target_include_directories(CSAS_Core
        INTERFACE
        .
        PRIVATE
        .
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Core PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Core
        PRIVATE
        ${Core_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Core
        PRIVATE
        CSAS_Platform
        CSAS_Renderer
        CSAS_ImGui


        )



