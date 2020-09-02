#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Window.h"

class Application {
 public:
  Application();
  ~Application();

  static Application* Get() { return m_Instance; }
  const std::unique_ptr<Window>& GetWindow() { return m_Window; }

  void Init();
  void Run();

 private:
  std::unique_ptr<Window> m_Window;
  bool m_Running = false;
  bool m_Dockspace = true;

  const static U32 WIDTH = 1600;
  const static U32 HEIGHT = 900;
  static Application* m_Instance;
};