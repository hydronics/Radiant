#include "rdpch.h"

#include "Buffers.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Radiant {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t count, float* vertices)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLVertexBuffer>(count, vertices);
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

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLVertexBuffer>(size);
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

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLIndexBuffer>(count, indices);
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
