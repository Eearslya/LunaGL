#include "pch.h"

#include "Application.h"
int main() {
  Application* app = new Application();
  app->Init();
  app->Run();
  delete app;
}