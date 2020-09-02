#pragma once

#include <glm/glm.hpp>

class Camera {
 public:
  Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
  ~Camera() = default;

  void SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateMatrices();
  }
  void SetPitch(float pitch) {
    m_Pitch = pitch;
    RecalculateMatrices();
  }
  void SetYaw(float yaw) {
    m_Yaw = yaw;
    RecalculateMatrices();
  }
  void SetCamera(const glm::vec3& position, float pitch, float yaw) {
    m_Position = position;
    m_Pitch = pitch;
    m_Yaw = yaw;
  }

  const glm::vec3& GetPosition() const { return m_Position; }
  const glm::vec3& GetForward() const { return m_Forward; }
  const float GetPitch() const { return m_Pitch; }
  const float GetYaw() const { return m_Yaw; }
  const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
  const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
  const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

  void HandleKeyboardInput(float dt);
  void InitMouseInput();
  void HandleMouseInput(float dt);
  void SetProjection(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

 private:
  void RecalculateMatrices();

  float m_LastMouseX;
  float m_LastMouseY;

  glm::mat4 m_ViewMatrix;
  glm::mat4 m_ProjectionMatrix;
  glm::mat4 m_ViewProjectionMatrix;

  glm::vec3 m_Position;
  glm::vec3 m_Forward;
  glm::vec3 m_Right;
  glm::vec3 m_Up;
  float m_Pitch;
  float m_Yaw;
};