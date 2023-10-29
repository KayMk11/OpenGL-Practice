#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum direction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class camera
{
    void updateCameraVectors()
    {
    }

public:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float sensitivity = 0.1f;
    float yaw = -90.f, pitch = 0.f;
    float fov;

    camera()
    {
        position = glm::vec3(0.f, 0.f, 0.f);
        forward = glm::vec3(0.f, 0.f, -1.f);
        up = glm::vec3(0.f, 1.f, 0.f);
        right = glm::vec3(1.f, 0.f, 0.f);
        fov = 45.f;
        updateCameraVectors();
    }
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + forward, up);
    }
    glm::mat4 getProjectionMatrix()
    {
        return glm::perspective(glm::radians(fov), (float)800 / (float)800, 0.1f, 100.0f);
    }
    void move(direction dir, float speed, float deltaTime)
    {
        if (dir == FORWARD)
            position += speed * deltaTime * forward;
        if (dir == BACKWARD)
            position -= speed * deltaTime * forward;
        if (dir == RIGHT)
            position -= speed * deltaTime * glm::normalize(glm::cross(up, forward));
        if (dir == LEFT)
            position += speed * deltaTime * glm::normalize(glm::cross(up, forward));
    }
    void rotate(float xoffset, float yoffset)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(direction);
    }
    void zoom(float level)
    {
        fov -= level;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
};
#endif