#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"
#include "world.h"
#include "camera.h"
#include "texture.h"


int WIDTH = 800 * 4 * .8;
int HEIGHT = 600 * 4 * .8;

World world;

// camera
Camera camera(glm::vec3(0.0f, 6.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
float dt = 0.0f;	
float lastFrame = 0.0f;

glm::vec3 playerPos;

Texture blockTexture;
    
unsigned int crosshairVBO;
unsigned int crosshairVAO;
unsigned int crosshairEBO;
std::vector<Shader> shaders;
GLFWwindow* window;


const float crosshairLineWidth = .002f;
const float crosshairLineLength = .018f;
float crosshairVerts[] = {
    -crosshairLineLength, crosshairLineWidth, 0.0f,
    -crosshairLineLength, -crosshairLineWidth,0.0f,
    crosshairLineLength, crosshairLineWidth,0.0f,
    crosshairLineLength, -crosshairLineWidth,0.0f,
    crosshairLineWidth, crosshairLineLength,0.0f,
    -crosshairLineWidth, crosshairLineLength,0.0f,
     crosshairLineWidth, -crosshairLineLength,0.0f,
    -crosshairLineWidth, -crosshairLineLength,0.0f,
};
unsigned int crosshairIndices[] = {
    0, 3, 1,
    0, 2, 3,
    4, 7, 5,
    4, 6, 7,
};


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.jumping = true;
    playerPos = camera.Position;
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaders[blockShader].use();
    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i].setMat4("view", view);
    }
    world.update(dt);

    // crosshair
    shaders[1].use();
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void update() {
    float currentFrame = static_cast<float>(glfwGetTime());
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    camera.update(dt);
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        camera.leftClicked = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        camera.rightClicked = true;
}

void cleanup() {
    glDeleteVertexArrays(1, &crosshairVAO);
    glDeleteBuffers(1, &crosshairVBO);
    glDeleteBuffers(1, &crosshairEBO);
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i].remove();
    }
    glfwTerminate();
} 

void loadVisuals() {
    glGenBuffers(1, &crosshairVBO);
    glGenBuffers(1, &crosshairEBO);
    glGenVertexArrays(1, &crosshairVAO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &crosshairVerts[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 12, &crosshairIndices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Shader
    shaders.push_back(Shader("block.vert", "block.frag"));
    shaders[blockShader].use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    shaders[blockShader].setMat4("projection", projection);
    shaders.push_back(Shader("screenSpace.vert", "solidColor.frag"));
    shaders[1].use();


    blockTexture.Load("terrain.png");

    world.setup();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
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


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

inline glm::vec3 MoveTowards(glm::vec3 start, glm::vec3 dest, float max) {
    glm::vec3 dif = dest - start;
    dif = glm::clamp(dif, -max, max);
    return start + dif;
}