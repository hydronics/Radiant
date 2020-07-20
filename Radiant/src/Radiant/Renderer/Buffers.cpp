#include "rdpch.h"

#include "Buffers.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Radiant {

	VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(size, vertices);
			}
			case RendererAPI::DirectX:
			{
				RD_CORE_ASSERT(false, "Only OpenGL supported for now!");
				return nullptr;
			}
			case RendererAPI::None:
			{
				RD_CORE_ASSERT(false, "'None' supplied as RendererAPI!  Not valid!");
			}
		}

		RD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t size, uint32_t* indices)
	{
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::OpenGL:
		{
			return new OpenGLIndexBuffer(size, indices);
		}
		case RendererAPI::None:
		case RendererAPI::DirectX:
		{
			RD_CORE_ASSERT(false, "Only OpenGL supported for now!");
			return nullptr;
		}
		}

		RD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}