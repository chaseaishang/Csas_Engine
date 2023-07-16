cmake_minimum_required(VERSION 3.25)

project(CSAS_Scene)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_Scene)
aux_source_directory(./ SCENE_DIR_LIB_SRCS)

target_include_directories(CSAS_Scene
        INTERFACE
        .
        PRIVATE
        .
        ${CSAS_ENGINE_SRC_PATH}
        ${ENGINE_INCLDE_DIR}/vendor
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Scene PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Scene
        PRIVATE
        ${SCENE_DIR_LIB_SRCS}
        )




