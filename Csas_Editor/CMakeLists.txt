cmake_minimum_required(VERSION 3.25)
project(Csas_Editor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	/home/chaseaishang/code/CSAS_Engine/output/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RLEASE 	/home/chaseaishang/code/CSAS_Engine/output/lib)
set(CMAKE_TOOLCHAIN_FILE /home/chaseaishang/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKGHEAD_PATH /home/chaseaishang/tools/vcpkg/installed/x64-linux/include)
set(CSAS_ENGINE_SRC_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/src)
set(CSAS_ENGINE_VENDOR_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/vendor)

add_compile_definitions(CSAS_PLATFORM_LINUX)
add_compile_definitions(CSAS_ENABLE_ASSERTS)
set(ENGINE_INCLDE_DIR /home/chaseaishang/code/CSAS_Engine/CSAS_Engine)
add_executable(Csas_Editor)
add_subdirectory(src/UI/)
add_subdirectory(src/utils)
add_subdirectory(src/Example/)
add_subdirectory(../CSAS_Engine CSAS_Engine_dir)
target_sources(Csas_Editor
        PRIVATE
        src/CsasEditorApp.cpp
        src/EditorLayer.cpp
        src/Panels/SceneHierarchyPanel.cpp
        src/Panels/SceneMaterialPanel.cpp
        )
target_include_directories(Csas_Editor
        PUBLIC
        src/Example
        src/UI
        ${ENGINE_INCLDE_DIR}/src
        ${ENGINE_INCLDE_DIR}/vendor)
target_link_libraries(Csas_Editor
        CSAS_Editor_UI
        CSAS_Engine
        CSAS_Editor_UTILS
        )