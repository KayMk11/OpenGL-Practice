#include<iostream>
#include<string>

#include "window_manager.h"
#include "model.h"
#include "camera.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Blending");

    gladLoadGL();

    InputManager im;
    Camera cam;

    im.add_keyboard_listener(cam);
    im.add_mouse_listener(cam);
    im.add_scroll_listener(cam);

    wm.set_input_manager(im);

    Shader shader("./shaders/model.vs", "./shaders/model.fs");
    Shader blend_shader("./shaders/model.vs", "./shaders/blend.fs");

    Model plane("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/plane.obj");
    Model grass("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/grass.obj");

    glEnable(GL_DEPTH_TEST);

    while(wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 projection = cam.getProjectionMatrix();
        glm::mat4 view = cam.getViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 plane_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.f, 0.0f));
        plane_model = glm::scale(plane_model, glm::vec3(10.f, 10.f, 10.f));
        shader.setMat4("model", plane_model);
        plane.draw(shader);

        blend_shader.use();
        blend_shader.setMat4("projection", projection);
        blend_shader.setMat4("view", view);
        
        glm::mat4 grass_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -.0f, -5.f));
        grass_model = glm::rotate(grass_model, glm::radians(90.f), glm::vec3(1., 0., 0.));

        blend_shader.setMat4("model", grass_model);
        grass.draw(blend_shader);
        wm.updateWindow();
    }
    wm.close();
    return 0;
}