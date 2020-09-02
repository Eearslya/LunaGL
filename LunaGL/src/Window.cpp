#include "pch.h"

#include "Window.h"

#include "Log.h"

#include <glad/glad.h>

#include "Input.h"
#include "Renderer/Renderer.h"
#include "examples/imgui_impl_glfw.h"

Window::Window(const std::string& title, U32 width, U32 height)
    : m_Window(nullptr), m_Title(title), m_Width(width), m_Height(height), m_ShouldClose(false) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  LOGI("GLFW Initialized.");

  LOGI("Creating window \"{}\" @ {} x {}", title, width, height);
  m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (m_Window == nullptr) {
    const char* description;
    int code = glfwGetError(&description);
    LOGF("GLFW Error {0}: {1}", code, description);
    ASSERT(false, "Failed to create GLFW window!");
    glfwTerminate();
    abort();
  }
  glfwMakeContextCurrent(m_Window);

  // Center window on the screen
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  int monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);
  int windowW, windowH;
  glfwGetWindowSize(m_Window, &windowW, &windowH);
  glfwSetWindowPos(m_Window, monitorX + (mode->width - windowW) / 2,
                   monitorY + (mode->height - windowH) / 2);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    ASSERT(false, "Failed to initialize Glad!");
    abort();
  }

  glfwSetWindowUserPointer(m_Window, this);

  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
    Window& instance = *(Window*)glfwGetWindowUserPointer(window);
    instance.OnWindowResized(width, height);
  });

  glfwSetKeyCallback(m_Window,
                     [](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
                       if (action == GLFW_PRESS) {
                         Input::OnKeyDown(keycode);
                       } else if (action == GLFW_RELEASE) {
                         Input::OnKeyUp(keycode);
                       }
                     });

  glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
    Input::OnMouseMove((float)xPos, (float)yPos);
  });

  ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
}

Window::~Window() {}

void Window::NewFrame() { ImGui_ImplGlfw_NewFrame(); }

void Window::Update() {
  ImGuiIO& io = ImGui::GetIO();

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  HandleInput();
  glfwSwapBuffers(m_Window);
  glfwPollEvents();

  m_ShouldClose = glfwWindowShouldClose(m_Window);
}

bool Window::Minimized() const {
  int w, h;
  glfwGetWindowSize(m_Window, &w, &h);

  return (w == 0) && (h == 0);
}

void Window::WaitForEvent() const { glfwWaitEvents(); }

void Window::SetMouseCaptured(bool captured) {
  glfwSetInputMode(m_Window, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::OnWindowResized(U32 width, U32 height) { Renderer::SetViewport(width, height); }

void Window::HandleInput() {}

void Window::Render() {}