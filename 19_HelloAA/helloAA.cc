#include<iostream>
#include<string>

#include"window_manager.h"
#include"camera.h"
#include"model.h"

int main()
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    WindowManager wm;
    wm.init(800, 800, "AntiAliasing");
    
    
    InputManager im;
    Camera camera;

    wm.set_input_manager(im);
    im.add_keyboard_listener(camera);
    im.add_mouse_listener(camera);

    gladLoadGL();
    glEnable(GL_MULTISAMPLE);  

    Shader shader("./shaders/model.vs", "./shaders/model.fs");
    Model cube("/home/kaymk11/Code/OpenGL-Practice/resources/models/cube.obj");

    glEnable(GL_DEPTH_TEST);

    while(wm.isWindowActive())
    {
        glClearColor(0.07, 0.13, 0.17, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -5.0f));

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        cube.draw(shader);

        wm.updateWindow();
    }

}