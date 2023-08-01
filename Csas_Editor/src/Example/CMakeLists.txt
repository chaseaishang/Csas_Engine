cmake_minimum_required(VERSION 3.25)
add_subdirectory(Example1/)
add_subdirectory(Example2/)
add_subdirectory(Example3/)
target_sources(Csas_Editor
        PRIVATE
        Example.cpp
        )


