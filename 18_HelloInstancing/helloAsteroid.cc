#include<iostream>
#include<string>

#include"window_manager.h"
#include"camera.h"
#include"model.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Asteroid field");

    Camera camera;

    InputManager im;
    wm.set_input_manager(im);
    im.add_keyboard_listener(camera);
    im.add_mouse_listener(camera);

    gladLoadGL();

    Shader shader("./shaders/instancing2.vs", "./shaders/instancing2.fs");
    Model planet("/home/kaymk11/Code/OpenGL-Practice/resources/models/planet/planet.obj");   
    Model asteroid("/home/kaymk11/Code/OpenGL-Practice/resources/models/rock/rock.obj");   

    glEnable(GL_DEPTH_TEST);

    unsigned int amount = 10000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];

    srand(glfwGetTime());

    float radius = 50.0;
    float offset = 2.5f;

    for (unsigned int  i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < asteroid.meshes.size(); i++)
    {
        unsigned int VAO = asteroid.meshes[i].VAO;
        glBindVertexArray(VAO);
        // vertex attributes
        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3); 
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }  
    while(wm.isWindowActive())
    {
        glClearColor(0.07, 0.13, 0.17, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.);
        model = glm::translate(model, glm::vec3(0.f, -3.f, -0.f));
        // model = glm::scale(model, glm::vec3(4.0, 4.0, 4.0));
        shader.setMat4("model", model);
        // planet.draw(shader);

        // for(unsigned int i = 0; i < amount; i++)
        // {
        //     shader.setMat4("model", modelMatrices[i]);
        //     asteroid.draw(shader);
        // }
        for(unsigned int i = 0; i < asteroid.meshes.size(); i++)
        {
            glBindVertexArray(asteroid.meshes[i].VAO);
            // glDrawArraysInstanced(GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            glDrawElementsInstanced( GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        }

        wm.updateWindow();
    }
    return 0;
}