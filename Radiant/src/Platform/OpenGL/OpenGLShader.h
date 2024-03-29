#pragma once

#include "Radiant/Renderer/Shader.h"
#include <glad/glad.h>

namespace Radiant {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& shader_filepath);
		OpenGLShader(const std::string& name, const std::string& vertex_shader_rawstring, const std::string& pixel_shader_rawstring);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_name; }

		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);

		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat(const std::string& name, float value);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);


		void SetMat4(const std::string& name, const glm::mat4& value) override;
		void SetMat3(const std::string& name, const glm::mat3& value) override;

		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat(const std::string& name, float value) override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);

	private:
		uint32_t m_renderer_id = 0;
		std::string m_name;
	};
}
