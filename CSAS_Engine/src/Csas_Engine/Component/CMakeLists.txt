cmake_minimum_required(VERSION 3.25)

project(CSAS_Component)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库
find_package(assimp CONFIG REQUIRED)
add_library (CSAS_Component)
aux_source_directory(./ COMPONENT_DIR_LIB_SRCS)

target_include_directories(CSAS_Component
        INTERFACE
        .
        PRIVATE
        .
        ${CSAS_ENGINE_SRC_PATH}
        ${ENGINE_INCLDE_DIR}/vendor
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_Component PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_Component
        PRIVATE
        ${COMPONENT_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Component
        PRIVATE
        assimp::assimp

        )
#Tiny_Obj_Loader


