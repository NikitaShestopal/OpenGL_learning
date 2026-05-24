#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

const char *vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTex;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}
)";

const char *borderFragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400.0f;
float lastY = 300.0f;

bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = (float) xpos;
        lastY = (float) ypos;
        firstMouse = false;
    }

    float xoffset = (float) xpos - lastX;
    float yoffset = lastY - (float) ypos;

    lastX = (float) xpos;
    lastY = (float) ypos;

    float sensitivity = 0.1f;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;

    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    const int windowWidth = 800;
    const int windowHeight = 600;

    GLFWwindow *window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        "Lab - 3D Interactive Cubes",
        NULL,
        NULL
    );

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);

    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    unsigned int borderFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        borderFragmentShader,
        1,
        &borderFragmentShaderSource,
        NULL
    );

    glCompileShader(borderFragmentShader);

    unsigned int borderShaderProgram = glCreateProgram();

    glAttachShader(borderShaderProgram, vertexShader);

    glAttachShader(borderShaderProgram, borderFragmentShader);

    glLinkProgram(borderShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(borderFragmentShader);

    float vertices[] = {

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void *) 0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void *) (3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    unsigned char texData[] = {
        255, 128, 0,
        0, 200, 255,
        0, 200, 255,
        255, 128, 0
    };

    unsigned int texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        2,
        2,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        texData
    );

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 1.0f, -3.0f),
        glm::vec3(-1.5f, -1.0f, -1.5f)
    };

    std::vector<glm::vec3> cubeScales = {
        glm::vec3(1.0f),
        glm::vec3(0.8f),
        glm::vec3(1.2f)
    };

    int activeCube = 0;

    double lastTime = glfwGetTime();

    float activeCubeAngle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        float deltaTime = (float) (currentTime - lastTime);

        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = 2.5f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(
                glm::cross(cameraFront, cameraUp)
            ) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(
                glm::cross(cameraFront, cameraUp)
            ) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            activeCube = 0;

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            activeCube = 1;

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            activeCube = 2;

        activeCubeAngle += 50.0f * deltaTime;

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT
        );

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float) windowWidth / (float) windowHeight,
            0.1f,
            100.0f
        );

        glBindVertexArray(VAO);

        glBindTexture(GL_TEXTURE_2D, texture);

        for (int i = 0; i < cubePositions.size(); i++) {
            glStencilFunc(GL_ALWAYS, 1, 0xFF);

            glStencilMask(0xFF);

            glUseProgram(shaderProgram);

            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, cubePositions[i]);

            if (i == activeCube) {
                model = glm::rotate(
                    model,
                    glm::radians(activeCubeAngle),
                    glm::vec3(0.5f, 1.0f, 0.0f)
                );
            }

            model = glm::scale(model, cubeScales[i]);

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(model)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram, "view"),
                1,
                GL_FALSE,
                glm::value_ptr(view)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram, "projection"),
                1,
                GL_FALSE,
                glm::value_ptr(projection)
            );

            glDrawArrays(GL_TRIANGLES, 0, 36);

            if (i == activeCube) {
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

                glStencilMask(0x00);

                glDisable(GL_DEPTH_TEST);

                glUseProgram(borderShaderProgram);

                glm::mat4 borderModel = glm::mat4(1.0f);

                borderModel = glm::translate(
                    borderModel,
                    cubePositions[i]
                );

                borderModel = glm::rotate(
                    borderModel,
                    glm::radians(activeCubeAngle),
                    glm::vec3(0.5f, 1.0f, 0.0f)
                );

                borderModel = glm::scale(
                    borderModel,
                    cubeScales[i] * 1.08f
                );

                glUniformMatrix4fv(
                    glGetUniformLocation(borderShaderProgram, "model"),
                    1,
                    GL_FALSE,
                    glm::value_ptr(borderModel)
                );

                glUniformMatrix4fv(
                    glGetUniformLocation(borderShaderProgram, "view"),
                    1,
                    GL_FALSE,
                    glm::value_ptr(view)
                );

                glUniformMatrix4fv(
                    glGetUniformLocation(borderShaderProgram, "projection"),
                    1,
                    GL_FALSE,
                    glm::value_ptr(projection)
                );

                glDrawArrays(GL_TRIANGLES, 0, 36);

                glStencilMask(0xFF);

                glEnable(GL_DEPTH_TEST);
            }
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);

    glDeleteProgram(shaderProgram);

    glDeleteProgram(borderShaderProgram);

    glfwTerminate();

    return 0;
}
