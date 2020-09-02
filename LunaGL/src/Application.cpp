#include "pch.h"

#include "Application.h"

#include "Log.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Input.h"
#include "Renderer/Renderer.h"

Application* Application::m_Instance = nullptr;

Application::Application() { m_Instance = this; }

Application::~Application() {}

void Application::Init() {
  LOGI("Initializing application...");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  io.FontDefault = io.Fonts->Fonts.back();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

  // Create our window
  m_Window = std::make_unique<Window>("LunaGL", WIDTH, HEIGHT);

  // Initialize the renderer
  Renderer::Init();
}

void Application::Run() {
  m_Running = true;
  static double lastUpdate = glfwGetTime();
  while (m_Running) {
    double now = glfwGetTime();
    float deltaTime = (float)(now - lastUpdate);
    lastUpdate = now;
    Input::Update();

    Renderer::NewFrame();
    m_Window->NewFrame();
    if (m_Window->Minimized()) {
      m_Window->WaitForEvent();
      continue;
    }
    ImGui::NewFrame();

    // Update scene here
    // activeScene->Update(deltaTime);

    // Create ImGUI dockspace
    if (m_Dockspace) {
      static ImGuiDockNodeFlags dockNodeFlags = ImGuiDockNodeFlags_None;
      constexpr ImGuiWindowFlags mainWindowFlags =
          ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

      ImGui::Begin("LunaGL", nullptr, mainWindowFlags);

      ImGui::PopStyleVar(3);

      // Dockspace
      ImGuiIO& io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace, ImVec2(0.0f, 0.0f), dockNodeFlags);
      }

      ImGui::Begin("Scenes");
      ImGui::End();  // Scenes

      ImGui::Begin("SceneOptions");
      // activeScene->ImGuiOptions();
      ImGui::End();  // SceneOptions

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      ImGui::Begin("Scene");

      static ImVec2 lastViewportSize = ImVec2(0, 0);
      auto viewportSize = ImGui::GetContentRegionAvail();
      if (lastViewportSize.x != viewportSize.x || lastViewportSize.y != lastViewportSize.y) {
        // activeScene->OnWindowResized(viewportSize.x, viewportSize.y);
      }
      // ImGui::Image((void*)activeScene->GetDisplayTexture(), viewportSize, {0, 1}, {1, 0});

      ImGui::End();  // Scene
      ImGui::PopStyleVar();

      ImGui::Begin("Renderer");
      ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                  ImGui::GetIO().Framerate);
      ImGui::Text("%d Drawcalls", Renderer::DrawCalls);
      static bool wireframe = false;
      if (ImGui::Checkbox("Wireframe View", &wireframe)) {
        Renderer::SetWireframe(wireframe);
      }
      if (ImGui::CollapsingHeader("Renderer Info")) {
        ImGui::Text("Vendor: %s", Renderer::GetVendor());
        ImGui::Text("Renderer: %s", Renderer::GetRenderer());
        ImGui::Text("Version: %s", Renderer::GetVersion());
      }
      ImGui::End();  // Renderer

      ImGui::End();  // LunaGL
    }

    // Rendering
    ImGui::Render();
    Renderer::Update();
    m_Window->Update();

    if (m_Window->ShouldClose()) {
      m_Running = false;
    }
  }
}
