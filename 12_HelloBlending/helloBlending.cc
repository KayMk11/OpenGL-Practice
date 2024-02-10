#include<iostream>
#include<string>
#include<vector>
#include<map>

#include"window_manager.h"
#include"shader.h"
#include"camera.h"
#include"model.h"


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
    Model window("/home/kaymk11/Code/OpenGL/OpenGL-Practice/resources/models/window.obj");

    std::vector<glm::vec3> window_positions = {
        {0.0, 0.0, -4.f},
        {.25, 0.0, -3.f},
        {-0.25, 0.0, -6.f},
        {0.0, 0.0, -7.f}
    };

    std::map<float, glm::vec3> sorted;
    for(int i = 0; i < window_positions.size(); i++)
    {
        float distance = glm::length(cam.position - window_positions[i]);
        sorted[distance] = window_positions[i];
    } 

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
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

        shader.use();
//        for(auto &pos : window_positions)
        for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
        {
            glm::mat4 window_model = glm::translate(glm::mat4(1.0), it->second);
            window_model = glm::rotate(window_model, glm::radians(90.f), glm::vec3(1., 0., 0.));
            shader.setMat4("model", window_model);
            window.draw(shader);
        }
        wm.updateWindow();
    }
    wm.close();
    return 0;
}

