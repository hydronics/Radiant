#include "rdpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Radiant {	

	////////////////////////////////////////////////////////////////
	//////  Vertex Buffer (OpenGL)  ////////////////////////////////
	////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t count, float* vertices)
		: VertexBuffer()
	{
		RD_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		: VertexBuffer()
	{
		RD_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		RD_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLVertexBuffer::Bind()
	{
		RD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLVertexBuffer::Unbind()
	{
		RD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		RD_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	////////////////////////////////////////////////////////////////
	//////  Index Buffer (OpenGL)  /////////////////////////////////
	////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, uint32_t* indices)
		: m_count(count)
	{
		RD_PROFILE_FUNCTION();

		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RD_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLIndexBuffer::Bind()
	{
		RD_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLIndexBuffer::Unbind()
	{
		RD_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
