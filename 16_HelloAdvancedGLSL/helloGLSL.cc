#include <iostream>
#include <string>

#include "window_manager.h"
#include "camera.h"
#include "input_manager.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{
    WindowManager wm;
    wm.init(800, 800, "GLSL");

    Camera camera;
    InputManager im;
    wm.set_input_manager(im);
    im.add_keyboard_listener(camera);
    im.add_mouse_listener(camera);
    im.add_scroll_listener(camera);

    gladLoadGL();

    Shader shader1("./shaders/model.vs", "./shaders/model1.fs");
    Shader shader2("./shaders/model.vs", "./shaders/model2.fs");
    Shader shader3("./shaders/model.vs", "./shaders/model3.fs");
    Shader shader4("./shaders/model.vs", "./shaders/model4.fs");

    Model cube("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/cube.obj");
    glEnable(GL_DEPTH_TEST);

    unsigned int uniformBlock1 = glGetUniformBlockIndex(shader1.ID, "Matrices");
    unsigned int uniformBlock2 = glGetUniformBlockIndex(shader2.ID, "Matrices");
    unsigned int uniformBlock3 = glGetUniformBlockIndex(shader3.ID, "Matrices");
    unsigned int uniformBlock4 = glGetUniformBlockIndex(shader4.ID, "Matrices");

    glUniformBlockBinding(shader1.ID, uniformBlock1, 0);
    glUniformBlockBinding(shader2.ID, uniformBlock2, 0);
    glUniformBlockBinding(shader3.ID, uniformBlock3, 0);
    glUniformBlockBinding(shader4.ID, uniformBlock4, 0);

    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    while (wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        // shader.setMat4("projection", projection);
        // shader.setMat4("view", view);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        shader1.use();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-.75, -.75, -5.));
        model = glm::scale(model, glm::vec3(.5, .5, .5));
        shader1.setMat4("model", model);
        cube.draw(shader1);

        shader2.use();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(.75, -.75, -5.));
        model = glm::scale(model, glm::vec3(.5, .5, .5));
        shader2.setMat4("model", model);
        cube.draw(shader2);

        shader3.use();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-.75, .75, -5.));
        model = glm::scale(model, glm::vec3(.5, .5, .5));
        shader3.setMat4("model", model);
        cube.draw(shader3);

        shader4.use();
        model = glm::translate(glm::mat4(1.0f), glm::vec3(.75, .75, -5.));
        model = glm::scale(model, glm::vec3(.5, .5, .5));
        shader4.setMat4("model", model);
        cube.draw(shader4);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        wm.updateWindow();
    }
    return 0;
}
