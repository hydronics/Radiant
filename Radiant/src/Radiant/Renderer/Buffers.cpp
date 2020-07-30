#include "rdpch.h"

#include "Buffers.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Radiant {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(size, vertices);
			}
			case RendererAPI::API::DirectX:
			{
				RD_CORE_ASSERT(false, "Only OpenGL supported for now!");
				return nullptr;
			}
			case RendererAPI::API::None:
			{
				RD_CORE_ASSERT(false, "'None' supplied as RendererAPI!  Not valid!");
			}
		}

		RD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* indices)
	{
		switch (Renderer::GetRendererAPI()) {
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLIndexBuffer>(size, indices);
		}
		case RendererAPI::API::None:
		case RendererAPI::API::DirectX:
		{
			RD_CORE_ASSERT(false, "Only OpenGL supported for now!");
			return nullptr;
		}
		}

		RD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
