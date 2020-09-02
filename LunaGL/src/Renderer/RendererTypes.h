#pragma once

#include <glm/glm.hpp>

#include "OpenGL/Texture.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

enum class TexType { Diffuse = 0, Specular = 1 };

struct TextureBinding {
  TexType Type;
  Ref<Texture> Texture;
};