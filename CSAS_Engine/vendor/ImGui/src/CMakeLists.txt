cmake_minimum_required(VERSION 3.25)

find_package(glfw3 CONFIG REQUIRED)
file(GLOB_RECURSE DIR_LIB_SRCS "*.cpp")
file(GLOB_RECURSE NODE_SRCS "main.cpp")
list(REMOVE_ITEM DIR_LIB_SRCS ${NODE_SRCS})

target_sources(ImGui
        PRIVATE
       ${DIR_LIB_SRCS}
        )
target_link_libraries(ImGui INTERFACE glfw)