#include "rdpch.h"

#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Radiant {

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Bool: return GL_BOOL;
		}

		RD_CORE_ASSERT(false, "Unknown Shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_renderer_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_renderer_id);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
	{
		glBindVertexArray(m_renderer_id);
		vertex_buffer->Bind();

		RD_CORE_ASSERT(vertex_buffer->GetLayout().GetElements().size(), "No Layout in the Vertex Buffer!");

		unsigned int index = 0;
		const auto& layout = vertex_buffer->GetLayout();
		for (const auto& elem : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				elem.GetComponentCount(),
				ShaderDataTypeToGLBaseType(elem.type),
				elem.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uint64_t)elem.offset
			);
			index++;
		}
		m_vertex_buffers.push_back(vertex_buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer)
	{
		glBindVertexArray(m_renderer_id);
		index_buffer->Bind();

		m_index_buffer = index_buffer;
	}

	const std::vector<std::shared_ptr<Radiant::VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_vertex_buffers;
	}

	const std::shared_ptr<Radiant::IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_index_buffer;
	}

}