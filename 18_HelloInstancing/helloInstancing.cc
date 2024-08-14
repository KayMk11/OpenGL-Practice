#include <iostream>
#include <string>

#include "window_manager.h"
#include "camera.h"
#include "model.h"

int main()
{
    WindowManager wm;
    wm.init(800, 800, "Instancing");
    Camera camera;
    InputManager im;
    wm.set_input_manager(im);

    im.add_keyboard_listener(camera);
    // im.add_mouse_listener(camera);

    gladLoadGL();

    Shader shader("./shaders/instancing.vs", "./shaders/instancing.fs");
    Model sphere("/home/kaymk11/Code/OpenGL-Practice/resources/models/sphere.obj");
    glEnable(GL_DEPTH_TEST);

    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.5f;
    for(int y = -100; y < 100; y +=20)
    {
        for(int x  = -100; x < 100; x += 20)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    shader.use();
    for(unsigned int i = 0; i < 100; i++)
    {
        shader.setVec2("offsets[" + std::to_string(i) + "]", translations[i]);
    } 

    while (wm.isWindowActive())
    {
        glClearColor(0.07, 0.13, 0.17, 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., -30.));
        // model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        shader.setMat4("model", model);
        shader.setFloat("time", glfwGetTime());
        for(Mesh &mesh : sphere.meshes)
        {
            mesh.material.activate_material(shader);
            glBindVertexArray(mesh.VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0, 100);

        }

        wm.updateWindow();
    }
    return 0;
}
