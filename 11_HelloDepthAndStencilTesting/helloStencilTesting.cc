#include <iostream>
#include <string>

#include "window_manager.h"
#include "model.h"
#include "camera.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Stencil Testing");
    gladLoadGL();

    InputManager im;
    Camera cam;

    im.add_keyboard_listener(cam);
    im.add_mouse_listener(cam);
    im.add_scroll_listener(cam);

    wm.set_input_manager(im);

    Shader shader("./shaders/model.vs", "./shaders/model.fs");
    Shader outline_shader("./shaders/model.vs", "./shaders/outline.fs");

    Model sphere("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/sphere.obj");
    Model cube("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/cube.obj");
    Model plane("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/plane.obj");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    while (wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        shader.use();
        glm::mat4 projection = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Draw Plane before enabling stencil test to make it not interfere
        glm::mat4 plane_model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.0f, 0.f));
        plane_model = glm::scale(plane_model, glm::vec3(50.f, 50.f, 50.f));
        shader.setMat4("model", plane_model);
        plane.draw(shader);
        
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF);

        glm::mat4 cube_model = glm::translate(glm::mat4(1.f), glm::vec3(-1.5, 0.f, -5.f));
        shader.setMat4("model", cube_model);
        cube.draw(shader);

        glm::mat4 sphere_model = glm::translate(glm::mat4(1.f), glm::vec3(.5, 0.f, -10.f));
        shader.setMat4("model", sphere_model);
        sphere.draw(shader);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer
        glDisable(GL_DEPTH_TEST);

        outline_shader.use();
        outline_shader.setMat4("projection", projection);
        outline_shader.setMat4("view", view);

        cube_model = glm::scale(cube_model, glm::vec3(1.05f, 1.05f, 1.05f));
        outline_shader.setMat4("model", cube_model);
        cube.draw(outline_shader);

        sphere_model = glm::scale(sphere_model, glm::vec3(1.05f, 1.05f, 1.05f));
        outline_shader.setMat4("model", sphere_model);
        sphere.draw(outline_shader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        wm.updateWindow();
    }
    wm.close();
    return 0;
}