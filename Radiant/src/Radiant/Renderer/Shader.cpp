#include "rdpch.h"

#include "Renderer.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Radiant {

	Ref<Shader> Shader::Create(const std::string& shader_filepath)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(shader_filepath);
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

	Ref<Shader> Shader::Create(const std::string& name, std::string& vertex_shader_rawstring, const std::string& pixel_shader_rawstring)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(name, vertex_shader_rawstring, pixel_shader_rawstring);
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

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		RD_CORE_ASSERT(!Exists(name), "Shader already found in ShaderLibrary!! : {0}", name);
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		RD_CORE_ASSERT(!Exists(name), "Shader already found in ShaderLibrary!! : {0}", name);
		m_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		RD_CORE_ASSERT(Exists(name), "Shader already found in ShaderLibrary!! : {0}", name);
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_shaders.find(name) != m_shaders.end();
	}

}
