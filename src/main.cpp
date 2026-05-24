#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main(){\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main(){\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int windowWidth = 800;
    const int windowHeight = 600;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lab - Interactive Rectangle", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Позиції (X, Y, Z)    // Текстурні координати (U, V)
    float vertices[] = {
        -0.3f,  0.2f, 0.0f,     0.0f, 1.0f, // В-Л
         0.3f,  0.2f, 0.0f,     1.0f, 1.0f, // В-П
        -0.3f, -0.2f, 0.0f,     0.0f, 0.0f, // Н-Л

         0.3f,  0.2f, 0.0f,     1.0f, 1.0f, // В-П
         0.3f, -0.2f, 0.0f,     1.0f, 0.0f, // Н-П
        -0.3f, -0.2f, 0.0f,     0.0f, 0.0f  // Н-Л
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned char texData[] = {
        255, 128, 0,    0, 200, 255,
        0, 200, 255,    255, 128, 0
    };

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

    float posX = 0.0f;
    float posY = 0.0f;
    float angle = 0.0f;
    float moveSpeed = 0.015f;

    double lastTime = glfwGetTime();
    const double timePerFrame = 1.0 / 60.0;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        if (currentTime - lastTime >= timePerFrame) {
            lastTime = currentTime;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  posX -= moveSpeed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) posX += moveSpeed;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    posY += moveSpeed;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  posY -= moveSpeed;

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            float ndcX = (float)(mouseX / windowWidth) * 2.0f - 1.0f;
            float ndcY = 1.0f - (float)(mouseY / windowHeight) * 2.0f;

            if (ndcX >= posX - 0.3f && ndcX <= posX + 0.3f &&
                ndcY >= posY - 0.2f && ndcY <= posY + 0.2f) {
                angle += 2.0f;
            }

            glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            glBindTexture(GL_TEXTURE_2D, texture);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
        }

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}