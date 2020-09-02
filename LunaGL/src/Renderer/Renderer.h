#pragma once

class Renderer {
 public:
  static void Init();
  static void NewFrame();
  static void Update();
  static void SetViewport(U32 width, U32 height);
  static void SetWireframe(bool enable);

  static const char* GetVendor();
  static const char* GetRenderer();
  static const char* GetVersion();

  static U32 DrawCalls;
};