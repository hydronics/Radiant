#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Radiant {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		// Set<Type><Size> e.g. SetMat4 - uniform/cbuffer API (hooks into material system)
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;

		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;

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