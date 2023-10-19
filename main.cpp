#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"
#include "world.h"
World world;

enum ShaderNames
{
    quadShader,
};

int WIDTH = 800 * 4 * .8;
int HEIGHT = 600 * 4 * .8;
float TRIANGLEVERTS[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
unsigned int TriangleVBO;
unsigned int TriangleVAO;
unsigned int QuadEBO;
std::vector<Shader> shaders;

GLFWwindow* window;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaders[quadShader].use();
    glBindVertexArray(TriangleVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadEBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void update() {
    processInput(window);

    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return true;
}

void cleanup() {
    glDeleteVertexArrays(1, &TriangleVAO);
    glDeleteBuffers(1, &TriangleVBO);
    glDeleteBuffers(1, &QuadEBO);
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i].remove();
    }
    glfwTerminate();
}

void loadVisuals() {
    // VBO and VAO and EBO
    glGenBuffers(1, &TriangleVBO);
    glGenBuffers(1, &QuadEBO);
    glGenVertexArrays(1, &TriangleVAO);
    glBindVertexArray(TriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLEVERTS), TRIANGLEVERTS, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    
    // shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // Shader
    shaders.push_back(Shader("screenSpace.vert", "solidColor.frag"));

}

int main()
{
    if (!setupWindow()) {
        return -1;
    }
    loadVisuals();
    while (!glfwWindowShouldClose(window))
    {
        update();
    }
    cleanup();
    return 0;
}