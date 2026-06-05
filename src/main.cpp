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
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void drawGrid(Shader& shader, float size, int cells, glm::mat4 view, glm::mat4 projection, float yOffset);

unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

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

    // ---- 1 ----
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Universal Model Viewer", primaryMonitor, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
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

    // ---- 2 ----
    Model ourModel("resources/Car/Car.obj");

    // ---- 3 ----
    float d = ourModel.modelRadius * 2.0f;

    camera.Position = glm::vec3(-d, d, d);

    camera.Yaw   = -45.0f;
    camera.Pitch = -35.0f;
    camera.ProcessMouseMovement(0, 0);

    float lightRadius = ourModel.modelRadius * 1.5f;
    float lightHeight = ourModel.maxBounds.y * 1.5f;
    if (lightHeight < 1.0f) lightHeight = 2.0f;

    // Рівень підлоги
    float floorY = ourModel.minBounds.y - ourModel.modelCenter.y;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Очищення екрана
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // ---- 4 ----
        float lightX = static_cast<float>(sin(glfwGetTime()) * lightRadius);
        float lightZ = static_cast<float>(cos(glfwGetTime()) * lightRadius);
        glm::vec3 lightPos = glm::vec3(lightX, lightHeight, lightZ);

        ourShader.setVec3("light.position", lightPos);
        ourShader.setVec3("light.ambient",  glm::vec3(0.15f, 0.15f, 0.15f));
        ourShader.setVec3("light.diffuse",  glm::vec3(1.5f, 1.5f, 1.5f));
        ourShader.setVec3("light.specular", glm::vec3(1.5f, 1.5f, 1.5f));

        ourShader.setFloat("light.constant",  1.0f);
        ourShader.setFloat("light.linear",    0.09f);
        ourShader.setFloat("light.quadratic", 0.032f);

        // ---- 5 ----
        drawGrid(ourShader, ourModel.modelRadius * 5.0f, 30, view, projection, floorY);

        // ---- 6 ----
        ourShader.use();
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, -ourModel.modelCenter);

        ourShader.setMat4("model", modelMatrix);
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Функція малювання координатної сітки
void drawGrid(Shader& shader, float size, int cells, glm::mat4 view, glm::mat4 projection, float yOffset) {
    std::vector<float> xLinesVertices;
    std::vector<float> zLinesVertices;
    float step = size / cells;

    for (int i = -cells; i <= cells; ++i) {
        xLinesVertices.push_back(i * step); xLinesVertices.push_back(yOffset); xLinesVertices.push_back(-size);
        xLinesVertices.push_back(i * step); xLinesVertices.push_back(yOffset); xLinesVertices.push_back(size);

        zLinesVertices.push_back(-size); zLinesVertices.push_back(yOffset); zLinesVertices.push_back(i * step);
        zLinesVertices.push_back(size);  zLinesVertices.push_back(yOffset); zLinesVertices.push_back(i * step);
    }

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setVec3("material.specular", glm::vec3(0.0f));

    // 🟢 ГОЛОВНИЙ ПРАПОРЕЦЬ: ставимо 0.0f, щоб шейдер вимкнув тіні для ліній
    shader.setFloat("material.shininess", 0.0f);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // --- КРОК А: Малюємо червоні лінії (вздовж Z) ---
    glBufferData(GL_ARRAY_BUFFER, xLinesVertices.size() * sizeof(float), &xLinesVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    shader.setVec3("material.ambient", glm::vec3(1.0f, 0.1f, 0.1f)); // Робимо колір яскравим
    shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.1f, 0.1f));
    glDrawArrays(GL_LINES, 0, xLinesVertices.size() / 3);

    // --- КРОК Б: Малюємо сині лінії (вздовж X) ---
    glBufferData(GL_ARRAY_BUFFER, zLinesVertices.size() * sizeof(float), &zLinesVertices[0], GL_STATIC_DRAW);

    shader.setVec3("material.ambient", glm::vec3(0.1f, 0.1f, 1.0f)); // Робимо колір яскравим
    shader.setVec3("material.diffuse", glm::vec3(0.1f, 0.1f, 1.0f));
    glDrawArrays(GL_LINES, 0, zLinesVertices.size() / 3);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    if (height == 0) height = 1;

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
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
