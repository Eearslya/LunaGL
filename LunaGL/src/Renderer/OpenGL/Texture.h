#pragma once

#include <glad/glad.h>

#include <string>

class Texture {
 public:
  Texture(U32 width, U32 height);
  Texture(const std::string& path);
  virtual ~Texture();

  U32 GetWidth() const { return m_Width; }
  U32 GetHeight() const { return m_Height; }

  void SetData(void* data, U32 size);

  void Bind(U32 slot = 0) const;

  static Ref<Texture> Create(U32 width, U32 height);
  static Ref<Texture> Create(const std::string& path);

 private:
  std::string m_Path;
  U32 m_Width, m_Height;
  U32 m_RendererId;
  GLenum m_InternalFormat;
  GLenum m_DataFormat;
};