cmake_minimum_required(VERSION 3.25)

project(CSAS_Editor_UI)
set(CMAKE_CXX_STANDARD 17)
set(VCPKGHEAD_PATH /home/chaseaishang/tools/vcpkg/installed/x64-linux/include)
set(CSAS_ENGINE_SRC_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/src)
set(CSAS_ENGINE_VENDOR_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/vendor)

add_library (CSAS_Editor_UI)
target_precompile_headers(CSAS_Editor_UI PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
aux_source_directory(. EDITOR_UI_DIR_LIB_SRCS)

target_include_directories(CSAS_Editor_UI
        PUBLIC
        INTERFACE
        ./
        PRIVATE
        ./
        ../Example
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        ${CSAS_ENGINE_VENDOR_PATH}
        )
target_sources(CSAS_Editor_UI
        PUBLIC
        ${EDITOR_UI_DIR_LIB_SRCS}
        )
target_link_libraries(CSAS_Editor_UI
        INTERFACE
        ImGui
        )
