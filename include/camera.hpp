#pragma once

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    enum class Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 front = glm::vec3(0.f, 0.f, -1.f), glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.f);

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    float getZoom() const { return zoom; }

    void processKeyboard(Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset)
    {
        zoom = std::clamp(zoom - yoffset, 1.f, 45.f);
    }

private:
    void updateCameraVectors();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 world_up;

    glm::vec3 up;
    glm::vec3 right;

    // euler Angles
    float yaw;
    float pitch;

    // camera options
    float move_speed;
    float mouse_sensitivity;
    float zoom;
};