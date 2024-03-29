#include "rdpch.h"

#include "Texture.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Radiant {

	Ref<Texture2d> Texture2d::Create(const std::string& texture_filepath)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLTexture2d>(texture_filepath);
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

	Ref<Texture2d> Texture2d::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::OpenGL:
		{
			return CreateRef<OpenGLTexture2d>(width, height);
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
