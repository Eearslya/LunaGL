#pragma once

#include <vector>

class Framebuffer {
 public:
  Framebuffer(U32 width, U32 height, U32 textureCount = 1);
  ~Framebuffer();

  void Bind();
  void Unbind();
  void BindTexture(U32 texture, U32 slot);
  void CopyDepthTo(Ref<Framebuffer> other);

  const U32 GetColorTexture(U32 index = 0) const { return m_ColorTextures[index]; }

 private:
  U32 m_RendererID;
  std::vector<U32> m_ColorTextures;
  U32 m_RenderObjectID;

  U32 m_Width;
  U32 m_Height;
};