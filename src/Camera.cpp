#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_worldUp(up),
      m_yaw(yaw),
      m_pitch(pitch),
      m_movementSpeed(3.0f),
      m_turnSpeed(60.0f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        m_position += m_front * velocity;
    if (direction == CameraMovement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == CameraMovement::LEFT)
        m_position -= m_right * velocity;
    if (direction == CameraMovement::RIGHT)
        m_position += m_right * velocity;
}

void Camera::processRotation(CameraRotation direction, float deltaTime)
{
    float angle = m_turnSpeed * deltaTime;
    if (direction == CameraRotation::UP)
        m_pitch += angle;
    if (direction == CameraRotation::DOWN)
        m_pitch -= angle;
    if (direction == CameraRotation::LEFT)
        m_yaw -= angle;
    if (direction == CameraRotation::RIGHT)
        m_yaw += angle;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    front.y = std::sin(glm::radians(m_pitch));
    front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
