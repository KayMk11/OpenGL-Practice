#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "window_manager.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
int main()
{
    WindowManager wm;
    wm.init(800, 800, "Hello Multiple Lights");
    gladLoadGL();

    Shader ourShader("./shaders/model.vs", "./shaders/model.fs");
    Camera cam;
    wm.setActiveCamera(&cam);
    Model ourModel("/home/kaymk11/Code/OpenGL/resources/models/sphere.obj");
    glEnable(GL_DEPTH_TEST);

    while (wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        ourShader.use();
        glm::mat4 projection = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0., 0., -10.));
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        ourModel.draw(ourShader);
        wm.updateWindow();
    }
    wm.close();
    return 0;
}