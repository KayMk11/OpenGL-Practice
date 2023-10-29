#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "camera.h"

bool firstMouse = true;
float lastx, lasty;
camera *activeCamera;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

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
    activeCamera->zoom((float)yoffset);
}
class window_manager
{
    GLFWwindow *window;
    int height, width;
    std::string title;
    void processInput(float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            activeCamera->move(FORWARD, .25f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            activeCamera->move(LEFT, .25f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            activeCamera->move(BACKWARD, .25f, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            activeCamera->move(RIGHT, .25f, deltaTime);
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
    void setActiveCamera(camera *cam)
    {
        activeCamera = cam;
    }
    int isWindowActive()
    {
        return !glfwWindowShouldClose(window);
    }
    void updateWindow(float deltaTime)
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(deltaTime);
    }
    void close()
    {
        glfwTerminate();
    }
};

#endif
