#include "rdpch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Radiant {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLVertexArray>();
			}
			case RendererAPI::API::Vulkan:
			case RendererAPI::API::Metal:
			case RendererAPI::API::DirectX:
			{
				RD_CORE_ASSERT(false, "Only OpenGL supported for now!  [{0}] was supplied.", Renderer::GetAPI());
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

}
