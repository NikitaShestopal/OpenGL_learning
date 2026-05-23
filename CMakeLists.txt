cmake_minimum_required(VERSION 3.30)
project(OpenGL_learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        glm
        glad
)

add_executable(OpenGL_learning
        src/main.cpp
)

target_link_libraries(OpenGL_learning ${ALL_LIBS})
