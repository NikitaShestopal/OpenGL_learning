#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

// Структура вершини згідно з ТЗ (Позиція, Текстурні координати, Нормаль)
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// Залишаємо структуру для сумісності з Assimp завантажувачем
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // Дані меша
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    // ДОДАНО: Зберігає індекс матеріалу з OBJ/MTL для зв'язку з Model.h
    unsigned int MaterialIndex;

    // Конструктор
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->MaterialIndex = 0; // Значення за замовчуванням

        setupMesh();
    }

    // Метод рендерингу (згідно з Варіантом Б, викликається без параметрів шейдера)
    void Draw() const {
        // Біндимо VAO меша
        glBindVertexArray(VAO);

        // Малюємо індексовану геометрію (Пункт 1 ТЗ: використання EBO є обов'язковим)
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

        // Відв'язуємо VAO
        glBindVertexArray(0);
    }

private:
    // Буфери
    unsigned int VBO, EBO;

    // Конфігурація буферів і атрибутів вершин
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Завантаження вершин у VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Завантаження індексів у EBO (Індексний буфер)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Атрибут 0: Позиція вершин (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Атрибут 1: Текстурні координати (vec2)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // Атрибут 2: Нормалі вершин (vec3)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glBindVertexArray(0);
    }
};

#endif