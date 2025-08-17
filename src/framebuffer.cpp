#include "framebuffer.hpp"
#include "debug.hpp"
#include <glad/glad.h>

Framebuffer::Framebuffer(uint width, uint height)
    : _width(width), _height(height) {
  create_framebuffer();
}

Framebuffer::~Framebuffer() {
  for (uint id : _color_attachments) {
    glDeleteTextures(1, &id);
  }
  for (uint id : _render_buffer_attachments) {
    glDeleteRenderbuffers(1, &id);
  }
  glDeleteFramebuffers(1, &_id);
}

void Framebuffer::bind(int target) {
  _target = target;
  glBindFramebuffer(target, _id);
}

void Framebuffer::unbind() { glBindFramebuffer(_target, 0); }

uint Framebuffer::width() const { return _width; }

uint Framebuffer::height() const { return _height; }

uint Framebuffer::id() { return _id; }

const std::array<uint, MAX_COLOR_ATTACHMENTS> &
Framebuffer::get_color_attachments() {
  return _color_attachments;
}

const std::array<uint, MAX_RENDERBUFFER_ATTACHMENTS> &
Framebuffer::get_rbo_attachments() {
  return _render_buffer_attachments;
}

uint Framebuffer::get_color_attachment_by_index(size_t index) {
  ASSERT(index < color_attachments_count(),
         "index is greater than the number of color attachments");
  return _color_attachments[index];
}

uint Framebuffer::get_rbo_attachment_by_index(size_t index) {
  ASSERT(index < rbo_attachments_count(),
         "index is greater than the number of renderbuffer attachments");
  return _render_buffer_attachments[index];
}

uint Framebuffer::color_attachments_count() const {
  return _n_color_attachments;
}
uint Framebuffer::rbo_attachments_count() const {
  return _n_renderbuffer_attachments;
}

void Framebuffer::create_color_attachment(
    const ColorAttachmentCreateInfo &cinfo) {
  bind();

  uint attachment_id = 0;

  glGenTextures(1, &attachment_id);
  glBindTexture(GL_TEXTURE_2D, attachment_id);

// HACK: MacOS does weird stuff. Have to multiply width and height by 2 when
// creating framebuffer stuff Read comments on learnopengl.com for more info
#ifdef __APPLE__
  glTexImage2D(GL_TEXTURE_2D, 0, cinfo.format, _width * 2, _height * 2, 0,
               cinfo.format, cinfo.type, NULL);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, cinfo.format, _width, _height, 0, cinfo.format,
               cinfo.type, NULL);
#endif
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  cinfo.min_texture_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  cinfo.mag_texture_filter);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0 + _n_color_attachments,
                         GL_TEXTURE_2D, attachment_id, 0);

  _color_attachments[_n_color_attachments] = attachment_id;
  _n_color_attachments++;

  unbind();
}

void Framebuffer::create_rbo_attachment(const RenderbufferCreateInfo &cinfo) {
  bind();

  uint rbo = 0;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  // 24 bit depth buffer and 8 bit stencil buffer
  // HACK: Comment in create_color_attachment applies here as well
#ifdef __APPLE__
  glRenderbufferStorage(GL_RENDERBUFFER, cinfo.format, _width * 2, _height * 2);
#else
  glRenderbufferStorage(GL_RENDERBUFFER, cinfo.format, _width, _height);
#endif
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // TODO: figure out how to deal with separate depth and stencil buffers
  if (cinfo.format == GL_DEPTH24_STENCIL8) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);
  }

  _render_buffer_attachments[_n_renderbuffer_attachments] = rbo;
  _n_renderbuffer_attachments++;

  unbind();
}

bool Framebuffer::is_complete() const {
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::create_framebuffer() { glGenFramebuffers(1, &_id); }
