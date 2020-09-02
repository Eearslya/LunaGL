#include "pch.h"

#include "Framebuffer.h"

#include "Log.h"

#include <glad/glad.h>

Framebuffer::Framebuffer(U32 width, U32 height, U32 textureCount)
    : m_Width(width), m_Height(height), m_RendererID(0) {
  ASSERT(width > 0);
  ASSERT(height > 0);
  ASSERT(textureCount > 0);

  glGenFramebuffers(1, &m_RendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

  std::vector<U32> attachments;
  m_ColorTextures.resize(textureCount);
  glGenTextures(textureCount, m_ColorTextures.data());
  for (unsigned int i = 0; i < textureCount; i++) {
    const U32 texture = m_ColorTextures[i];

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
    attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
  }
  glDrawBuffers(textureCount, attachments.data());

  glGenRenderbuffers(1, &m_RenderObjectID);
  glBindRenderbuffer(GL_RENDERBUFFER, m_RenderObjectID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                            m_RenderObjectID);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOGE("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
  glDeleteTextures(static_cast<unsigned int>(m_ColorTextures.size()), m_ColorTextures.data());
  glDeleteRenderbuffers(1, &m_RenderObjectID);
  glDeleteFramebuffers(1, &m_RendererID);
}

void Framebuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
  glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::BindTexture(U32 texture, U32 slot) {
  ASSERT(texture < m_ColorTextures.size());

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_ColorTextures[texture]);
}

void Framebuffer::CopyDepthTo(Ref<Framebuffer> other) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other->m_RendererID);
  glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT,
                    GL_NEAREST);
}