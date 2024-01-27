#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "window_manager.h"
#include "shader.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void dumpMatrix(glm::mat4 &mat)
{
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << mat[0][0] << "\t\t\t" << mat[1][0] << "\t\t\t" << mat[2][0] << "\t\t\t" << mat[3][0] << "\n"
              << mat[0][1] << "\t\t\t" << mat[1][1] << "\t\t\t" << mat[2][1] << "\t\t\t" << mat[3][1] << "\n"
              << mat[0][2] << "\t\t\t" << mat[1][2] << "\t\t\t" << mat[2][2] << "\t\t\t" << mat[3][2] << "\n"
              << mat[0][3] << "\t\t\t" << mat[1][3] << "\t\t\t" << mat[2][3] << "\t\t\t" << mat[3][3] << "\n";
    std::cout << "---------------------------------------------------------------------------\n";
}

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Hello Camera");

    gladLoadGL();
    // glViewport(0, 0, 800, 800);
    Shader ourShader("./shaders/transform.vs", "./shaders/transform.fs");

    float vertices[] = {
        // positions      // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int height, width, nrchannels;
    unsigned char *data = stbi_load("../resources/textures/wall.jpg", &width, &height, &nrchannels, 0);
    std::cout << "height : " << height << " width : " << width << " nrchannels : " << nrchannels << std::endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    InputManager im;
    Camera cam;
    im.add_keyboard_listener(cam);
    im.add_mouse_listener(cam);
    im.add_scroll_listener(cam);
    wm.set_input_manager(im);
    while (wm.isWindowActive())
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glActiveTexture(GL_TEXTURE0); // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader.setInt("ourTexture", 0);
        glBindVertexArray(VAO);

        glm::mat4 projection = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();
        dumpMatrix(view);
        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));
        model = glm::rotate(model, glm::radians(45.f), glm::vec3(.5f, .2f, 0.f));

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        wm.updateWindow();
        // std::cout << cam.position.x << " " << cam.position.y << " " << cam.position.z << std::endl;
    }
    wm.close();
}