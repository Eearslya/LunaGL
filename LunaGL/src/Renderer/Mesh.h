#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "RendererTypes.h"

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
       const std::vector<TextureBinding>& textures);

  void Draw(const Ref<Shader>& shader) const;

  std::vector<TextureBinding> Textures;

 private:
  Ref<VertexArray> m_VAO;
};