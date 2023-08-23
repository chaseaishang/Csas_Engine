cmake_minimum_required(VERSION 3.25)

project(CSAS_System)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_System)
aux_source_directory(./ SYSTEM_DIR_LIB_SRCS)

target_include_directories(CSAS_System
        INTERFACE
        ../Component
        PRIVATE
        ../Component
        ${CSAS_ENGINE_SRC_PATH}
        ${ENGINE_INCLDE_DIR}/vendor
        ${VCPKGHEAD_PATH}
        )
target_precompile_headers(CSAS_System PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_System
        PRIVATE
        ${SYSTEM_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_System PUBLIC CSAS_Component)



