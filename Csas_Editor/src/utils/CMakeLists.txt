cmake_minimum_required(VERSION 3.25)

project(CSAS_Editor_UTILS)
set(CMAKE_CXX_STANDARD 17)
set(VCPKGHEAD_PATH /home/chaseaishang/tools/vcpkg/installed/x64-linux/include)
set(CSAS_ENGINE_SRC_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/src)
set(CSAS_ENGINE_VENDOR_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/vendor)

add_library (CSAS_Editor_UTILS)
target_precompile_headers(CSAS_Editor_UI PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
aux_source_directory(. EDITOR_UTILS_DIR_LIB_SRCS)

target_include_directories(CSAS_Editor_UTILS
        PUBLIC
        INTERFACE
        ./
        PRIVATE
        ./
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        ${CSAS_ENGINE_VENDOR_PATH}
        )
target_sources(CSAS_Editor_UTILS
        PUBLIC
        ${EDITOR_UTILS_DIR_LIB_SRCS}
        )

