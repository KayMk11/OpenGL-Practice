#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "window_manager.h"
#include <iostream>
#include <cmath>

int main()
{
    window_manager wm;
    wm.init(800, 800, "Hello WM");
    gladLoadGL();

    glViewport(0, 0, 800, 800);
    shader ourShader("./shaders/default.vs", "./shaders/default.fs");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    unsigned int VAO, VBO;
    // Generate VAO
    glGenVertexArrays(1, &VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Assign data to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Tell opengl how to read the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // once opengl knows how to read VBO, enable it so that opengl uses it
    glEnableVertexAttribArray(0);

    // unbind VBO and VAO so that we dont mistakenly make changes to it;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("red", std::sin(glfwGetTime() * 3));
        ourShader.setFloat("green", std::sin(glfwGetTime() * 5));
        ourShader.setFloat("blue", std::sin(glfwGetTime() * 7));

        glBindVertexArray(VAO);

        // tell opengl to draw bound VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);

        wm.updateWindow();
    }
    wm.close();
    return 0;
}