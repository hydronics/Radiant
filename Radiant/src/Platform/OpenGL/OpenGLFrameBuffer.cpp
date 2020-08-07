#include "rdpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Radiant {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& props)
		: m_props(props)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_renderer_id);
		glDeleteTextures(1, &m_color_attachment_tex);
		glDeleteTextures(1, &m_depth_attachment);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_renderer_id != 0)
		{
			glDeleteFramebuffers(1, &m_renderer_id);
			glDeleteTextures(1, &m_color_attachment_tex);
			glDeleteTextures(1, &m_depth_attachment);
		}

		glCreateFramebuffers(1, &m_renderer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment_tex);
		glBindTexture(GL_TEXTURE_2D, m_color_attachment_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_props.width, m_props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment_tex, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
		glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_props.width, m_props.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		RD_CORE_ASSERT(status, "FrameBuffer did not complete.  Something went wrong!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
		glViewport(0, 0, m_props.width, m_props.height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_props.width = width;
		m_props.height = height;
		Invalidate();
	}

}
