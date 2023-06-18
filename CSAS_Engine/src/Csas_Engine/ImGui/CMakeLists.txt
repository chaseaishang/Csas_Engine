cmake_minimum_required(VERSION 3.25)

project(CSAS_ImGui)
#CSAS_PLATFORM_LINUX

set(CMAKE_CXX_STANDARD 17)




# 生成链接库

add_library (CSAS_ImGui)
aux_source_directory(. IMGUI_DIR_SRCS)
message(STATUS 666${IMGUI_DIR_SRCS})
target_include_directories(CSAS_ImGui
        INTERFACE
        ./
        PRIVATE
        ./
        ${CSAS_ENGINE_VENDOR_PATH}
        ${CSAS_ENGINE_SRC_PATH}
        ${VCPKGHEAD_PATH}
        )

target_precompile_headers(CSAS_ImGui PRIVATE ${CSAS_ENGINE_SRC_PATH}/Csas_Engine/Csaspch.h)
target_sources(CSAS_ImGui
        PRIVATE
        ${IMGUI_DIR_SRCS}
        )
target_link_libraries(CSAS_ImGui INTERFACE ImGui)