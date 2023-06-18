cmake_minimum_required(VERSION 3.25)


aux_source_directory(. DIR_LIB_SRCS)
target_sources(CSAS_Engine
        PRIVATE
        ${DIR_LIB_SRCS}
        )
message(STATUS 888${DIR_LIB_SRCS})
target_include_directories(CSAS_Engine
        PUBLIC
        ${CSAS_ENGINE_SRC_PATH}
        /home/chaseaishang/tools/vcpkg/installed/x64-linux/include
        )
target_precompile_headers(CSAS_Engine PRIVATE Csaspch.h)
find_package(glad REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog CONFIG REQUIRED)


#spdlog_header_only
target_link_libraries(CSAS_Engine PRIVATE spdlog::spdlog_header_only)
add_subdirectory(Events/)
add_subdirectory(ImGui/)
add_subdirectory(Platform/)
add_subdirectory(Renderer/)
target_link_libraries(CSAS_Engine PRIVATE ImGui)
target_link_libraries(CSAS_Engine PRIVATE glad::glad)
target_link_libraries(CSAS_Engine PRIVATE glfw)
target_link_libraries(CSAS_Engine PRIVATE CSAS_Events)
target_link_libraries(CSAS_Engine PRIVATE CSAS_ImGui)
target_link_libraries(CSAS_Engine PRIVATE CSAS_Platform)
target_link_libraries(CSAS_Engine PRIVATE CSAS_Renderer)