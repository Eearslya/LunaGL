#include "pch.h"

#include "Camera.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "Input.h"

Camera::Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
    : m_Position(0, 0, 0),
      m_Pitch(0),
      m_Yaw(-90.0f),
      m_ViewMatrix(1.0f),
      m_ProjectionMatrix(
          glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane)) {
  RecalculateMatrices();
  auto [x, y] = Input::GetMouse();
  m_LastMouseX = x;
  m_LastMouseY = y;
}

void Camera::HandleKeyboardInput(float dt) {
  const float cameraSpeed = 5.0f * dt;
  if (Input::KeyDown(GLFW_KEY_W)) m_Position += cameraSpeed * m_Forward;
  if (Input::KeyDown(GLFW_KEY_S)) m_Position -= cameraSpeed * m_Forward;
  if (Input::KeyDown(GLFW_KEY_A)) m_Position -= m_Right * cameraSpeed;
  if (Input::KeyDown(GLFW_KEY_D)) m_Position += m_Right * cameraSpeed;
}

void Camera::InitMouseInput() {
  auto [x, y] = Input::GetMouse();
  m_LastMouseX = x;
  m_LastMouseY = y;
}

void Camera::HandleMouseInput(float dt) {
  auto [x, y] = Input::GetMouse();
  float deltaX = m_LastMouseX - x;
  float deltaY = m_LastMouseY - y;
  m_LastMouseX = x;
  m_LastMouseY = y;

  const float sensitivity = 0.1f;
  deltaX *= sensitivity;
  deltaY *= sensitivity;

  m_Yaw -= deltaX;
  m_Pitch += deltaY;

  m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
  if (m_Yaw > 360.0f) {
    m_Yaw -= 360.0f;
  } else if (m_Yaw < 0.0f) {
    m_Yaw += 360.0f;
  }

  RecalculateMatrices();
}

void Camera::SetProjection(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) {
  m_ProjectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);
  RecalculateMatrices();
}

void Camera::RecalculateMatrices() {
  m_Forward =
      glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)), sin(glm::radians(m_Pitch)),
                sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
  m_Forward = glm::normalize(m_Forward);
  m_Right = glm::normalize(glm::cross(m_Forward, {0, 1, 0}));
  m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

  m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);

  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
