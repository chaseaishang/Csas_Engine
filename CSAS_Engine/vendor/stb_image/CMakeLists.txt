cmake_minimum_required(VERSION 3.25)
project(Stb_image)
add_library(Stb_image)

target_include_directories(Stb_image
        INTERFACE
        .
        PRIVATE
        .
        )
target_sources(Stb_image
        PRIVATE
        stb_image.cpp
        )


