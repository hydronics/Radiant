#pragma once

#include <string>

namespace Radiant {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& shader_filepath);
		static Ref<Shader> Create(const std::string& name, std::string& vertex_shader_rawstring, const std::string& pixel_shader_rawstring);

	private:
		uint32_t m_renderer_id = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Get(const std::string& name);
	private:
		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}