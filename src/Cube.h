#pragma once // 🟢 Сучасний захист від нескінченного зациклення файлів

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"

class Cube {
public:
    unsigned int VAO, VBO;
    int materialMode;

    Cube() {
        materialMode = 1; // Золото за замовчуванням
        setupGeometry();
    }

    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Draw(Shader &shader, float floorY, float modelRadius) {
        shader.use();

        if (materialMode == 1) { // ЗОЛОТО
            shader.setVec3("material.ambient",  glm::vec3(0.24725f, 0.1995f, 0.0745f));
            shader.setVec3("material.diffuse",  glm::vec3(0.75164f, 0.60648f, 0.22648f));
            shader.setVec3("material.specular", glm::vec3(0.62828f, 0.5558f, 0.36606f));
            shader.setFloat("material.shininess", 51.2f);
        }
        else if (materialMode == 2) { // БРОНЗА
            shader.setVec3("material.ambient",  glm::vec3(0.2125f, 0.1275f, 0.054f));
            shader.setVec3("material.diffuse",  glm::vec3(0.714f, 0.4284f, 0.18144f));
            shader.setVec3("material.specular", glm::vec3(0.39355f, 0.27191f, 0.16672f));
            shader.setFloat("material.shininess", 25.6f);
        }
        else if (materialMode == 3) { // СРІБЛО
            shader.setVec3("material.ambient",  glm::vec3(0.19225f, 0.19225f, 0.19225f));
            shader.setVec3("material.diffuse",  glm::vec3(0.50754f, 0.50754f, 0.50754f));
            shader.setVec3("material.specular", glm::vec3(0.50827f, 0.50827f, 0.50827f));
            shader.setFloat("material.shininess", 51.2f);
        }
        else { // ПЛАСТИК
            shader.setVec3("material.ambient",  glm::vec3(0.25f, 0.25f, 0.25f));
            shader.setVec3("material.diffuse",  glm::vec3(0.9f, 0.9f, 0.85f));
            shader.setVec3("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));
            shader.setFloat("material.shininess", 8.0f);
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(modelRadius * 1.5f, floorY + 0.5f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

private:
    void setupGeometry() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }
};