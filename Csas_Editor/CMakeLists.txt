cmake_minimum_required(VERSION 3.25)
project(SandBox)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions(CSAS_PLATFORM_LINUX)
add_compile_definitions(CSAS_ENABLE_ASSERTS)
set(ENGINE_INCLDE_DIR /home/chaseaishang/code/CSAS_Engine/CSAS_Engine)
add_subdirectory(../CSAS_Engine CSAS_Engine_dir)

add_executable(SandBox src/SandBoxTest.cpp src/Sandbox2D.cpp
                        src/ExampleLayer.cpp)
target_include_directories(SandBox PRIVATE   ${ENGINE_INCLDE_DIR}/src ${ENGINE_INCLDE_DIR}/vendor)
target_link_libraries(SandBox CSAS_Engine)