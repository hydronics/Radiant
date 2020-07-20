#include "rdpch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Radiant {

	Radiant::VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRendererAPI()) {
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexArray();
			}
			case RendererAPI::Vulkan:
			case RendererAPI::Metal:
			case RendererAPI::DirectX:
			{
				RD_CORE_ASSERT(false, "Only OpenGL supported for now!  [{0}] was supplied.", Renderer::GetRendererAPI());
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

}