cmake_minimum_required(VERSION 3.25)

project(CSAS_Engine)
#CSAS_PLATFORM_LINUX
set(CMAKE_TOOLCHAIN_FILE /home/chaseaishang/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKGHEAD_PATH /home/chaseaishang/tools/vcpkg/installed/x64-linux/include)
set(CSAS_ENGINE_SRC_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/src)
set(CSAS_ENGINE_VENDOR_PATH /home/chaseaishang/code/CSAS_Engine/CSAS_Engine/vendor)
set(CMAKE_CXX_STANDARD 17)



#aux_source_directory(./src/Csas_Engine DIR_LIB_SRCS)

# 生成链接库

add_library (CSAS_Engine "")
add_subdirectory(vendor/ImGui/)
add_subdirectory(src/Csas_Engine/)

