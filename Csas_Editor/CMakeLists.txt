cmake_minimum_required(VERSION 3.25)
project(Csas_Editor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions(CSAS_PLATFORM_LINUX)
add_compile_definitions(CSAS_ENABLE_ASSERTS)
set(ENGINE_INCLDE_DIR /home/chaseaishang/code/CSAS_Engine/CSAS_Engine)
add_executable(Csas_Editor)
add_subdirectory(src/Example/)
add_subdirectory(../CSAS_Engine CSAS_Engine_dir)
target_sources(Csas_Editor
        PRIVATE
        src/CsasEditorApp.cpp
        src/EditorLayer.cpp
        src/Panels/SceneHierarchyPanel.cpp)
target_include_directories(Csas_Editor
        PUBLIC
        src/Example
        ${ENGINE_INCLDE_DIR}/src
        ${ENGINE_INCLDE_DIR}/vendor)
target_link_libraries(Csas_Editor
        CSAS_Engine
        )