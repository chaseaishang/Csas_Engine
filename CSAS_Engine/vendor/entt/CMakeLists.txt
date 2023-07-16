cmake_minimum_required(VERSION 3.25)

project(entt)

add_library(entt)
set_target_properties(entt PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )
target_include_directories(entt
        INTERFACE
        ./include
        )
target_sources(entt PRIVATE entt.cpp)


