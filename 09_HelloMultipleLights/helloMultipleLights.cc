#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "window_manager.h"
#include "shader.h"
#include "texture.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Hello Multiple Lights");

    gladLoadGL();
    Shader lightShader("./shaders/cube.vs", "./shaders/light.fs");
    Shader cubeShader("./shaders/cube.vs", "./shaders/cube.fs");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    InputManager im;
    Camera cam;
    im.add_keyboard_listener(cam);
    im.add_mouse_listener(cam);
    im.add_scroll_listener(cam);
    wm.set_input_manager(im);
    cam.position = glm::vec3(.0f, .0f, 3.0f);
    auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    Texture diffuseMap("../resources/textures/container2.png", "diffuse");
    Texture specularMap("../resources/textures/container2_specular.png", "specular");

    while (wm.isWindowActive())
    {
        /* code */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightShader.use();
        glBindVertexArray(VAO);

        glm::mat4 projection = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();

        glm::mat4 model(1.0f);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setVec3("lightColor", lightColor);
        for (int i = 0; i < 4; i++)
        {
            model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(.1f, .1f, .1f));
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        cubeShader.use();
        cubeShader.setInt("material.diffuse", 0);
        cubeShader.setInt("material.specular", 1);
        cubeShader.setFloat("material.shininess", 64.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap.ID);

        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);
        cubeShader.setVec3("viewPos", cam.position);

        cubeShader.setVec3("dirLight.direction", 0.f, 0.f, -1.f);
        cubeShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        cubeShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        cubeShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        cubeShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        cubeShader.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[0].constant", 1.0f);
        cubeShader.setFloat("pointLight[0].linear", 0.09f);
        cubeShader.setFloat("pointLight[0].quadratic", 0.032f);

        cubeShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        cubeShader.setVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[1].constant", 1.0f);
        cubeShader.setFloat("pointLight[1].linear", 0.09f);
        cubeShader.setFloat("pointLight[1].quadratic", 0.032f);

        cubeShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        cubeShader.setVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[2].constant", 1.0f);
        cubeShader.setFloat("pointLight[2].linear", 0.09f);
        cubeShader.setFloat("pointLight[2].quadratic", 0.032f);

        cubeShader.setVec3("pointLight[3].position", pointLightPositions[3]);
        cubeShader.setVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[3].constant", 1.0f);
        cubeShader.setFloat("pointLight[3].linear", 0.09f);
        cubeShader.setFloat("pointLight[3].quadratic", 0.032f);

        cubeShader.setVec3("spotLight.position", cam.position);
        cubeShader.setVec3("spotLight.direction", cam.forward);
        cubeShader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("spotLight.constant", 1.0f);
        cubeShader.setFloat("spotLight.linear", 0.09f);
        cubeShader.setFloat("spotLight.quadratic", 0.032f);
        cubeShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("spotLight.outercutoff", glm::cos(glm::radians(17.5f)));

        for (int i = 0; i < 10; i++)
        {
            model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        wm.updateWindow();
    }
}