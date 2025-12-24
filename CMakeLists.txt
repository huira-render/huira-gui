cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH 
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup"
)
include(CustomVcpkgSetup)

project(imgui_app)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies via vcpkg
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Main application
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    imgui::imgui
)