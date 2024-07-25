#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "utils.hpp"
#include "sphere.h"
#include "camera.hpp"

int width, height;

constexpr std::size_t numVBOs{4};

GLuint VAO, VBO[numVBOs], EBO;

GLuint texture_sun, texture_earth, texture_moon;

Sphere mySphere(48);
ShaderProgram renderingProgram;
Camera camera{glm::vec3(0.0f, 0.0f, 20.0f)};

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    static bool firstMouse = true;
    static float lastX = width / 2.f;
    static float lastY = height / 2.f;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
    }
}

void setupVertices()
{
    std::vector<int> ind;
    std::transform(mySphere.getIndices().begin(), mySphere.getIndices().end(), std::back_inserter(ind), [](size_t i) -> int
                   { return static_cast<int>(i); });

    std::vector<float> pvalues, tvalues, nvalues;

    const auto &vert = mySphere.getVertices();
    const auto &tex = mySphere.getTexCoords();
    const auto &norm = mySphere.getNormals();

    auto numVertices = mySphere.getNumVertices();
    for (size_t i = 0; i < numVertices; i++)
    {
        pvalues.push_back(vert.at(i).x);
        pvalues.push_back(vert.at(i).y);
        pvalues.push_back(vert.at(i).z);

        tvalues.push_back(tex.at(i).s);
        tvalues.push_back(tex.at(i).t);

        nvalues.push_back(norm.at(i).x);
        nvalues.push_back(norm.at(i).y);
        nvalues.push_back(norm.at(i).z);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(numVBOs, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), pvalues.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), tvalues.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), nvalues.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), ind.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void display(GLFWwindow *window, double currentTime)
{
    float factor = static_cast<float>(currentTime);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = camera.getViewMatrix();
    renderingProgram.setUniformValue("view", view);

    auto projection = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    renderingProgram.setUniformValue("projection", projection);

    renderingProgram.setUniformValue("lightPos", glm::vec3{0.f, 0.f, 0.f});
    renderingProgram.setUniformValue("lightColor", glm::vec3{1.f, 1.f, 1.f});

    // sun
    glm::mat4 model{1.f};
    model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));                         // sun position
    auto sun_model = glm::rotate(model, factor * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f)); // 太阳自转
    renderingProgram.setUniformValue("model", sun_model);
    renderingProgram.setUniformValue("isLight", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_sun);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(mySphere.getNumIndices()), GL_UNSIGNED_INT, 0);

    renderingProgram.setUniformValue("isLight", 0);

    // earth
    model = glm::translate(model, glm::vec3(std::cos(factor) * 8.0f, std::sin(factor) * 6.0f, 0.f)); // earth position
    auto earth_model = glm::rotate(model, factor * 5, glm::vec3(0.0f, 0.0f, 1.0f));                  // 地球自转
    earth_model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    renderingProgram.setUniformValue("model", earth_model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_earth);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(mySphere.getNumIndices()), GL_UNSIGNED_INT, 0);

    // moon
    model = glm::translate(model, glm::vec3(std::cos(2 * factor) * 2.0f, std::sin(2 * factor) * 2.0f, 0.f)); // moon position
    model = glm::rotate(model, factor, glm::vec3(0.0f, 0.0f, 1.0f));                                         // 月球自转
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
    renderingProgram.setUniformValue("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_moon);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(mySphere.getNumIndices()), GL_UNSIGNED_INT, 0);
}

int main(int, char **)
{
    assert(glfwInit() == GLFW_TRUE);
    try
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        auto window = glfwCreateWindow(800, 600, "Sun Earth Moon System", nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window\n";
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int w, int h)
                                       {    width = w;    height = h;    glViewport(0, 0, width, height); });
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
                              { camera.processMouseScroll(static_cast<float>(yoffset)); });

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cout << "Failed to initialize GLAD\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        glfwGetFramebufferSize(window, &width, &height);

        texture_sun = utils::loadTexture("assets/sun.jpg");
        texture_earth = utils::loadTexture("assets/earth.jpg");
        texture_moon = utils::loadTexture("assets/moon.jpg");

        setupVertices();

        renderingProgram.addShaderSourceFromFile("shader/vertShader.glsl", ShaderProgram::ShaderType::VertexShader);
        renderingProgram.addShaderSourceFromFile("shader/fragShader.glsl", ShaderProgram::ShaderType::FragmentShader);
        renderingProgram.link();
        renderingProgram.use();

        float last_time = static_cast<float>(glfwGetTime());

        while (!glfwWindowShouldClose(window))
        {
            float delta_time = static_cast<float>(glfwGetTime()) - last_time;
            last_time = static_cast<float>(glfwGetTime());
            processInput(window, delta_time);
            display(window, glfwGetTime());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    glfwTerminate();
    return 0;
}
