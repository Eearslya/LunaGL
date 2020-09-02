#include "pch.h"

#include "Buffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const std::vector<float>& vertices) : m_RendererID(0) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices) : m_RendererID(0) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

Ref<VertexBuffer> VertexBuffer::Create(const std::vector<float>& vertices) {
  return CreateRef<VertexBuffer>(vertices);
}

IndexBuffer::IndexBuffer(const std::vector<U32>& indices) : m_RendererID(0), m_Count(0) {
  m_Count = static_cast<U32>(indices.size());
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), indices.data(),
               GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

Ref<IndexBuffer> IndexBuffer::Create(const std::vector<U32>& indices) {
  return CreateRef<IndexBuffer>(indices);
}
