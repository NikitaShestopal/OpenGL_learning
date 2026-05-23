#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main(){\n"
"   gl_Position = vec4(aPos, 1.0);\n"
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

    GLFWwindow* window = glfwCreateWindow(900, 600, "Lab - 2.3", NULL, NULL);
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

    // Позиція (X, Y, Z) і тут Текстурні координати (U, V)
    float vertices[] = {
        // === ПРЯМОКУТНИК 1
        -0.8f,  0.4f, 0.0f,   0.0f, 1.0f, // В-Л
        -0.3f,  0.4f, 0.0f,   1.0f, 1.0f, // В-П
        -0.8f, -0.4f, 0.0f,   0.0f, 0.0f, // Н-Л

        -0.3f,  0.4f, 0.0f,   1.0f, 1.0f, // В-П
        -0.3f, -0.4f, 0.0f,   1.0f, 0.0f, // Н-П
        -0.8f, -0.4f, 0.0f,   0.0f, 0.0f, // Н-Л

        // === ПРЯМОКУТНИК 2
        -0.25f,  0.4f, 0.0f,  0.0f, 1.0f,
         0.25f,  0.4f, 0.0f,  1.0f, 1.0f,
        -0.25f, -0.4f, 0.0f,  0.0f, 0.0f,

         0.25f,  0.4f, 0.0f,  1.0f, 1.0f,
         0.25f, -0.4f, 0.0f,  1.0f, 0.0f,
        -0.25f, -0.4f, 0.0f,  0.0f, 0.0f,

        // === ПРЯМОКУТНИК 3
         0.3f,  0.4f, 0.0f,   0.0f, 1.0f,
         0.8f,  0.4f, 0.0f,   1.0f, 1.0f,
         0.3f, -0.4f, 0.0f,   0.0f, 0.0f,

         0.8f,  0.4f, 0.0f,   1.0f, 1.0f,
         0.8f, -0.4f, 0.0f,   1.0f, 0.0f,
         0.3f, -0.4f, 0.0f,   0.0f, 0.0f
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

    unsigned char texData1[] = {
        255, 0, 0,    255, 255, 0,
        255, 255, 0,  255, 0, 0
    };
    unsigned char texData2[] = {
        0, 255, 255,  255, 0, 255,
        255, 0, 255,  0, 255, 255
    };
    unsigned char texData3[] = {
        0, 0, 0,          255, 255, 255,
        255, 255, 255,    0, 0, 0
    };

    unsigned int texture1, texture2, texture3;

    // 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST дасть чіткі кубики
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texData1);

    // 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texData2);

    // 3
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, texData3);

    // рендеринг
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // вершини 0-6
        glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // вершини 6-12
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawArrays(GL_TRIANGLES, 6, 6);

        // вершини 12-18
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawArrays(GL_TRIANGLES, 12, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очищення пам'яті
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glDeleteTextures(1, &texture3);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}