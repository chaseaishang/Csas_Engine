cmake_minimum_required(VERSION 3.25)




target_include_directories(CSAS_Engine
        INTERFACE
        ${CSAS_ENGINE_SRC_PATH}
        /home/chaseaishang/tools/vcpkg/installed/x64-linux/include
        )
target_precompile_headers(CSAS_Engine INTERFACE Csaspch.h)
find_package(glad REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog CONFIG REQUIRED)


#spdlog_header_only
target_link_libraries(CSAS_Engine INTERFACE spdlog::spdlog_header_only)
add_subdirectory(Platform/)
add_subdirectory(Core/)
add_subdirectory(Component/)
add_subdirectory(Events/)
add_subdirectory(ImGui/)
add_subdirectory(Scene/)
add_subdirectory(System/)
add_subdirectory(Renderer/)
add_subdirectory(Utils/)
target_link_libraries(CSAS_Engine INTERFACE ImGui)
target_link_libraries(CSAS_Engine INTERFACE entt)
target_link_libraries(CSAS_Engine INTERFACE Stb_image)
target_link_libraries(CSAS_Engine INTERFACE glad::glad)
target_link_libraries(CSAS_Engine INTERFACE glfw)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Platform)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Core)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Events)
target_link_libraries(CSAS_Engine INTERFACE CSAS_ImGui)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Renderer)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Scene)
target_link_libraries(CSAS_Engine INTERFACE CSAS_System)
target_link_libraries(CSAS_Engine INTERFACE CSAS_Utils)
