cmake_minimum_required(VERSION 3.30)
project(OpenGL_learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "")

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")

find_package(OpenGL REQUIRED)

add_subdirectory(external)

add_executable(OpenGL_learning
        src/main.cpp
        src/Shader.h
        src/Mesh.h
        src/Model.h
        src/Camera.h
)

target_include_directories(OpenGL_learning PRIVATE
        external/glad/include
        external/glm
        external/glfw/include
        external/assimp/include
)

target_link_libraries(OpenGL_learning
        PRIVATE
        OpenGL::GL
        glfw
        glm::glm
        glad
        assimp
)