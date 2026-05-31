#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Engine - Blinn-Phong Car", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Shader ourShader("src/vertex_shader.glsl", "src/fragment_shader.glsl");

    // ---- КРОК 1: ЗАВАНТАЖЕННЯ МОДЕЛИ МАШИНКИ ----
    Model ourModel("resources/Car/Car.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);

        // ---- КРОК 2: НАЛАШТУВАННЯ МАТЕРІАЛУ СУПЕРКАРА (Пункт 3 ТЗ) ----
        ourShader.setVec3("material.ambient",  glm::vec3(0.1f, 0.0f, 0.0f));
        ourShader.setVec3("material.diffuse",  glm::vec3(1.0f, 0.0f, 0.0f)); // Яскраво-червона фарба кузова
        ourShader.setVec3("material.specular", glm::vec3(0.7f, 0.7f, 0.7f)); // Дзеркальні білі відблиски лаку
        ourShader.setFloat("material.shininess", 128.0f);                    // Гострий, концентрований блік Блінна-Фонга

        // ---- ТОЧКОВЕ СВІТЛО (Обертання навколо машинки - Пункт 3 ТЗ) ----
        float lightX = static_cast<float>(sin(glfwGetTime()) * 3.0);
        float lightZ = static_cast<float>(cos(glfwGetTime()) * 3.0);
        glm::vec3 lightPos = glm::vec3(lightX, 2.5f, lightZ);

        ourShader.setVec3("light.position", lightPos);
        ourShader.setVec3("light.ambient",  glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setVec3("light.diffuse",  glm::vec3(1.0f, 1.0f, 1.0f)); // Чисте біле світло ліхтаря
        ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Коефіцієнти затухання світла (Attenuation)
        ourShader.setFloat("light.constant",  1.0f);
        ourShader.setFloat("light.linear",    0.09f);
        ourShader.setFloat("light.quadratic", 0.032f);

        // Матриці камери (Пункт 2 ТЗ)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // ---- КРОК 3: МАТРИЦЯ МОДЕЛІ (ВИПРАВЛЕНО) ----
        glm::mat4 model = glm::mat4(1.0f);

        // 1. Повертаємо позиціонування (опускаємо на 0.5 вниз і відсуваємо на 2.0 вперед)
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));

        // 2. Ставимо ОДИН нормальний масштаб для .obj файлу (1.0f)
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        ourShader.setMat4("model", model);

        // Рендеринг індексованої геометрії (Виправлено: викликаємо без аргументів)
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}