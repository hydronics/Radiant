#include "rdpch.h"

#include "Renderer.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Radiant {

	Shader* Shader::Create(const std::string& vertex_shader_file, const std::string& pixel_shader_file)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(vertex_shader_file, pixel_shader_file);
		}
		case RendererAPI::API::Vulkan:
		case RendererAPI::API::Metal:
		case RendererAPI::API::DirectX:
		{
			RD_CORE_ASSERT(false, "Only OpenGL supported for now!  [{0}] was supplied.", Renderer::GetRendererAPI());
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