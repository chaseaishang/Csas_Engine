cmake_minimum_required(VERSION 3.25)

project(CSAS_Utils)
set(CMAKE_CXX_STANDARD 17)


add_library (CSAS_Utils)
target_precompile_headers(CSAS_Editor_UI PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
aux_source_directory(. UTILS_DIR_LIB_SRCS)

target_include_directories(CSAS_Utils
        PUBLIC
        INTERFACE
        ./
        PRIVATE
        ./
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )
target_sources(CSAS_Utils
        PUBLIC
        ${UTILS_DIR_LIB_SRCS}
        )

