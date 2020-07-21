#pragma once

#include "Renderer/VertexArray.h"

namespace Radiant {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
		const Ref<IndexBuffer>& GetIndexBuffer() const override;

	private:
		uint32_t m_renderer_id;
		std::vector<Ref<VertexBuffer>> m_vertex_buffers;
		Ref<IndexBuffer> m_index_buffer;
	};

}