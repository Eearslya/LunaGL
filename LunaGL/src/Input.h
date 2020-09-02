#pragma once

#include <utility>

class Input {
 public:
  Input() = default;
  ~Input() = default;

  static void Update();
  static void SetMouseCaptured(bool captured);

  static void OnKeyDown(int keycode);
  static void OnKeyUp(int keycode);
  static void OnMouseMove(float x, float y);

  static bool KeyDown(int keycode);
  static bool KeyPressed(int keycode);
  static std::pair<float, float> GetMouse();

 private:
  static bool m_Keys[1024];
  static bool m_KeyPress[1024];
  static float m_MouseX;
  static float m_MouseY;
};