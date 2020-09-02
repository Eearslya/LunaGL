#include "pch.h"

#include "Renderer.h"

#include <glad/glad.h>

#include <imgui.h>

#include "examples/imgui_impl_opengl3.h"

void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar* message, const void* userParam) {
  if (type == GL_DEBUG_TYPE_OTHER) {
    return;
  }

  fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);

  if (type == GL_DEBUG_TYPE_ERROR) {
    __debugbreak();
  }
}

U32 Renderer::DrawCalls = 0;

void Renderer::Init() {
  ImGui_ImplOpenGL3_Init("#version 410");

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLMessageCallback, 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  LOGI("OpenGL initialized.");
}

void Renderer::NewFrame() {
  DrawCalls = 0;
  ImGui_ImplOpenGL3_NewFrame();
}

void Renderer::Update() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }

void Renderer::SetViewport(U32 width, U32 height) { glViewport(0, 0, width, height); }

void Renderer::SetWireframe(bool enabled) {
  glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}

const char* Renderer::GetVendor() { return reinterpret_cast<const char*>(glGetString(GL_VENDOR)); }

const char* Renderer::GetRenderer() {
  return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  ;
}

const char* Renderer::GetVersion() {
  return reinterpret_cast<const char*>(glGetString(GL_VERSION));
  ;
}
