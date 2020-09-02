#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window {
 public:
  Window(const std::string& title, U32 width, U32 height);
  ~Window();

  void NewFrame();
  void Update();
  bool ShouldClose() const { return m_ShouldClose; }
  bool Minimized() const;
  void WaitForEvent() const;
  void SetMouseCaptured(bool captured);

  void OnWindowResized(U32 width, U32 height);

 private:
  void HandleInput();
  void Render();

  bool m_ShouldClose;
  GLFWwindow* m_Window;
  U32 m_Width;
  U32 m_Height;
  std::string m_Title;
};