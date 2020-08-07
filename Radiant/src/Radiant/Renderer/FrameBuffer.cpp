#include "rdpch.h"

#include "FrameBuffer.h"
#include "Radiant/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Radiant {

	Radiant::Ref<Radiant::FrameBuffer> FrameBuffer::Create(const FrameBufferProperties& props)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLFrameBuffer>(props);
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

}
