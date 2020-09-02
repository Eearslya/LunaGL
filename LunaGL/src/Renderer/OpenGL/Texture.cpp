#include "pch.h"

#include "Texture.h"

#include "Log.h"

#include "stb_image.h"

Texture::Texture(U32 width, U32 height) : m_Width(width), m_Height(height) {
  m_InternalFormat = GL_RGBA8;
  m_DataFormat = GL_RGBA;

  glGenTextures(1, &m_RendererId);
  glBindTexture(GL_TEXTURE_2D, m_RendererId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);
}

Texture::Texture(const std::string& path) : m_Path(path), m_RendererId(0), m_Height(0), m_Width(0) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  ASSERT(data, "Failed to load image!");
  m_Width = width;
  m_Height = height;

  if (channels == 4) {
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;
  } else if (channels == 3) {
    m_InternalFormat = GL_RGB8;
    m_DataFormat = GL_RGB;
  }

  ASSERT(m_InternalFormat & m_DataFormat, "Image format not supported!");

  glGenTextures(1, &m_RendererId);
  glBindTexture(GL_TEXTURE_2D, m_RendererId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE,
               data);

  stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &m_RendererId); }

void Texture::SetData(void* data, U32 size) {
  U32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;
  ASSERT(size == m_Width * m_Height * bpp, "Data must include entire texture!");
  glBindTexture(GL_TEXTURE_2D, m_RendererId);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(U32 slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererId);
}

Ref<Texture> Texture::Create(U32 width, U32 height) { return CreateRef<Texture>(width, height); }

Ref<Texture> Texture::Create(const std::string& path) { return CreateRef<Texture>(path); }
