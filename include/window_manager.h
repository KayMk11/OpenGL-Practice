#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "camera.h"

bool firstMouse = true;
float lastx, lasty;
Camera *activeCamera;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (activeCamera == NULL)
        return;
    if (firstMouse)
    {
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
        return;
    }
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;
    activeCamera->rotate(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (activeCamera == NULL)
        return;
    activeCamera->zoom((float)yoffset);
}
class WindowManager
{
    GLFWwindow *window;
    int height, width;
    float deltaTime = 0.f;
    float lastFrame = 0.f;
    std::string title;
    void processInput(float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (activeCamera == NULL)
            return;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            activeCamera->move(FORWARD, .5f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            activeCamera->move(LEFT, .5f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            activeCamera->move(BACKWARD, .5f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            activeCamera->move(RIGHT, .5f, deltaTime);
    }

public:
    int init(int height, int width, std::string title)
    {
        this->height = height;
        this->width = width;
        this->title = title;

        if (!glfwInit())
            return false;

        window = glfwCreateWindow(height, width, title.c_str(), NULL, NULL);

        if (!window)
        {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        return true;
    }
    void setActiveCamera(Camera *cam)
    {
        activeCamera = cam;
    }
    int isWindowActive()
    {
        return !glfwWindowShouldClose(window);
    }
    float updateWindow()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(deltaTime);

        return deltaTime;
    }
    void close()
    {
        glfwTerminate();
    }
};

#endif
