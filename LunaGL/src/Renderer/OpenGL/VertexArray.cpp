#include "pch.h"

#include "VertexArray.h"

#include "Log.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
  }

  ASSERT(false, "Unknown ShaderDataType!");
  return 0;
}

Ref<VertexArray> VertexArray::Create() { return CreateRef<VertexArray>(); }

VertexArray::VertexArray() : m_RendererId(0) { glGenVertexArrays(1, &m_RendererId); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_RendererId); }

void VertexArray::Bind() const { glBindVertexArray(m_RendererId); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
  ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

  glBindVertexArray(m_RendererId);
  vertexBuffer->Bind();

  const auto& layout = vertexBuffer->GetLayout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(m_VertexBufferIndex);
    glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                          ShaderDataTypeToOpenGLBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                          (const void*)element.Offset);
    m_VertexBufferIndex++;
  }

  m_VertexBuffers.push_back(vertexBuffer);
  glBindVertexArray(0);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
  glBindVertexArray(m_RendererId);
  indexBuffer->Bind();
  m_IndexBuffer = indexBuffer;
  glBindVertexArray(0);
}