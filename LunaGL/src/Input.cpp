#include "pch.h"

#include "Input.h"

#include <memory>

#include "Application.h"

bool Input::m_Keys[1024] = {false};
bool Input::m_KeyPress[1024] = {false};
float Input::m_MouseX = 0.0f;
float Input::m_MouseY = 0.0f;

void Input::Update() {}

void Input::SetMouseCaptured(bool captured) {
  Application::Get()->GetWindow()->SetMouseCaptured(captured);
}

void Input::OnKeyDown(int keycode) {
  if (keycode > 0 && keycode < 1024) {
    m_Keys[keycode] = true;
    m_KeyPress[keycode] = true;
  }
}

void Input::OnKeyUp(int keycode) {
  if (keycode > 0 && keycode < 1024) {
    m_Keys[keycode] = false;
    m_KeyPress[keycode] = false;
  }
}

void Input::OnMouseMove(float x, float y) {
  m_MouseX = x;
  m_MouseY = y;
}

bool Input::KeyDown(int keycode) { return m_Keys[keycode]; }

bool Input::KeyPressed(int keycode) {
  if (m_KeyPress[keycode]) {
    m_KeyPress[keycode] = false;
    return true;
  }
  return false;
}

std::pair<float, float> Input::GetMouse() { return std::pair<float, float>(m_MouseX, m_MouseY); }
