add_subdirectory(glfw)

add_subdirectory(glm)

add_library(glad
        glad/src/glad.c
)

target_include_directories(glad PUBLIC glad/include)