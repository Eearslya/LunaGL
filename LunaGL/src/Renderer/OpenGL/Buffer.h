#pragma once

#include "Log.h"

#include <vector>

#include "Renderer/RendererTypes.h"

enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

static U32 ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
  }

  ASSERT(false, "Unknown ShaderDataType!");
  return 0;
}

struct BufferElement {
  std::string Name;
  ShaderDataType Type;
  U32 Size;
  size_t Offset;
  bool Normalized;

  BufferElement() = default;
  BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
      : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

  U32 GetComponentCount() const {
    switch (Type) {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Mat3:
        return 3 * 3;
      case ShaderDataType::Mat4:
        return 4 * 4;
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
    }

    ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }
};

class BufferLayout {
 public:
  BufferLayout() {}
  BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {
    CalculateOffsetsAndStride();
  }

  inline U32 GetStride() const { return m_Stride; }
  inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

  std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
  std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

 private:
  void CalculateOffsetsAndStride() {
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements) {
      element.Offset = offset;
      offset += element.Size;
      m_Stride += element.Size;
    }
  }

  std::vector<BufferElement> m_Elements;
  U32 m_Stride = 0;
};

class VertexBuffer {
 public:
  VertexBuffer(const std::vector<float>& vertices);
  VertexBuffer(const std::vector<Vertex>& vertices);
  VertexBuffer(const VertexBuffer&) = delete;
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

  const BufferLayout& GetLayout() const { return m_Layout; }
  void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

  static Ref<VertexBuffer> Create(const std::vector<float>& vertices);

 private:
  U32 m_RendererID;
  BufferLayout m_Layout;
};

class IndexBuffer {
 public:
  IndexBuffer(const std::vector<U32>& indices);
  IndexBuffer(const IndexBuffer&) = delete;
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  U32 GetCount() const { return m_Count; }

  static Ref<IndexBuffer> Create(const std::vector<U32>& indices);

 private:
  U32 m_RendererID;
  U32 m_Count;
};