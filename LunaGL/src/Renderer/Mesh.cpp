#include "pch.h"

#include "Mesh.h"

#include "Application.h"
#include "OpenGL/Texture.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
           const std::vector<TextureBinding>& textures)
    : Textures(textures) {
  m_VAO = CreateRef<VertexArray>();

  Ref<VertexBuffer> vbo = CreateRef<VertexBuffer>(vertices);
  vbo->SetLayout({{ShaderDataType::Float3, "inPosition"},
                  {ShaderDataType::Float3, "inNormal"},
                  {ShaderDataType::Float2, "inTexCoords"}});

  Ref<IndexBuffer> ibo = CreateRef<IndexBuffer>(indices);

  m_VAO->AddVertexBuffer(vbo);
  m_VAO->SetIndexBuffer(ibo);
}

void Mesh::Draw(const Ref<Shader>& shader) const {
  unsigned int diffuse = 1;
  unsigned int specular = 1;
  unsigned int textureIndex = 0;

  for (const auto& texture : Textures) {
    std::string index;
    if (texture.Type == TexType::Diffuse) {
      index = std::to_string(diffuse++);
      shader->SetInt("uMaterial.textureDiffuse" + index, textureIndex);
    } else if (texture.Type == TexType::Specular) {
      index = std::to_string(specular++);
      shader->SetInt("uMaterial.textureSpecular" + index, textureIndex);
    }

    texture.Texture->Bind(textureIndex++);
  }

  m_VAO->Bind();
  glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
}
