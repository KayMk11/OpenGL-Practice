#include<iostream>
#include<string>

#include"window_manager.h"
#include"shader.h"
#include"camera.h"
#include"model.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Blinn Phong");
    InputManager im;

    wm.set_input_manager(im);

    gladLoadGL();
    
    Camera camera;
    im.add_keyboard_listener(camera);
    im.add_mouse_listener(camera);


    Shader shader("./shaders/model.vs", "./shaders/model.fs");

    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    Texture tex("/home/kaymk11/Code/OpenGL-Practice/resources/textures/wall.jpg", "");

    shader.use();
    shader.setVec3("lightPos", 0., .1, 0.);
    shader.setVec3("lightColor", 1., 1., 1.);

    bool blinn = false, blinnKeyPressed = false;

    while (wm.isWindowActive())
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projection", camera.getProjectionMatrix());
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("model", glm::translate(glm::mat4(1.), glm::vec3(0., -.5f, 0.)));
        shader.setVec3("viewPos", camera.position);

        if (glfwGetKey(wm.window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed) 
        {
            blinn = !blinn;
            blinnKeyPressed = true;
        }
        if (glfwGetKey(wm.window, GLFW_KEY_B) == GLFW_RELEASE) 
        {
            blinnKeyPressed = false;
        }
        if(blinn)
            shader.setBool("blinn", true);
        else
            shader.setBool("blinn", false);

        glBindTexture(GL_TEXTURE_2D, tex.ID);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        wm.updateWindow();
    }

    wm.close();
    return 0;
    
}
