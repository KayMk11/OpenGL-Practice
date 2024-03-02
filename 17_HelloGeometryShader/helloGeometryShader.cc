#include <iostream>
#include <string>

#include "window_manager.h"
#include "camera.h"
#include "model.h"
#include "input_manager.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Geometry shader");
    Camera camera;
    InputManager im;
    wm.set_input_manager(im);
    im.add_keyboard_listener(camera);
    im.add_mouse_listener(camera);
    im.add_scroll_listener(camera);

    gladLoadGL();

    Shader shader("./shaders/model.vs", "./shaders/model.fs", "./shaders/model.gs");
    // Shader shader("./shaders/model.vs", "./shaders/model.fs", "");

    Model sphere("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/sphere.obj");
    glEnable(GL_DEPTH_TEST);

    while (wm.isWindowActive())
    {
        glClearColor(0.07, 0.13, 0.17, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., -5.));
        shader.setMat4("model", model);
        shader.setFloat("time", glfwGetTime());
        sphere.draw(shader);
        wm.updateWindow();
    }
    return 0;
}
