#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 world_up, float yaw, float pitch)
    : position{position},
      front{front},
      world_up{world_up},
      yaw{yaw}, pitch{pitch},
      move_speed(5.f),
      mouse_sensitivity(0.1f),
      zoom(45.f)
{
    updateCameraVectors();
}

void Camera::processKeyboard(Movement direction, float deltaTime)
{
    float distance = move_speed * deltaTime;

    switch (direction)
    {
    case Movement::FORWARD:
        position += front * distance;
        break;
    case Movement::BACKWARD:
        position -= front * distance;
        break;
    case Movement::LEFT:
        position -= right * distance;
        break;
    case Movement::RIGHT:
        position += right * distance;
        break;
    default:
        break;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        pitch = std::clamp(pitch, -89.f, 89.f);
    }

    // update front, right and up vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
