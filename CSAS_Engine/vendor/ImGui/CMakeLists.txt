cmake_minimum_required(VERSION 3.25)
set(CMAKE_TOOLCHAIN_FILE /home/chaseaishang/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(ImGui)
message(STATUS "    begin")


message( STATUS "glfw3_FOUND = ${glfw3_DIR}" )

#macro(print_all_variables)
#    message(STATUS"print_all_variables------------------------------------------{")
#    get_cmake_property(_variableNames VARIABLES)
#    foreach (_variableName ${_variableNames})
#        message(STATUS"${_variableName}=${${_variableName}}")
#    endforeach()
#    message(STATUS"print_all_variables------------------------------------------}")
#endmacro()
#print_all_variables()
#message( STATUS "glfw3_FOUND = ${glfw3_FOUND}" )
#message( STATUS "glfw3_VERSION = ${glfw3_VERSION}" )
#message( STATUS "glfw3_VERSION_MAJOR = ${glfw3_VERSION_MAJOR}" )
#message( STATUS "glfw3_VERSION_MINOR = ${glfw3_VERSION_MINOR}" )



add_library(ImGui)

#include_directories(${OpenCV_INCLUDE_DIRS})
target_include_directories(ImGui
        INTERFACE
        include
        PRIVATE
        include
        ${VCPKGHEAD_PATH}
        )


add_subdirectory(src/)
