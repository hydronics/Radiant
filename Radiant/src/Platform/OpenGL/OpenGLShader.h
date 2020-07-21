#pragma once

#include "Radiant/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Radiant {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertex_shader_file, const std::string& pixel_shader_file);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);

		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat(const std::string& name, float value);

		void UploadUniformInt(const std::string& name, int value);

	private:
		uint32_t m_renderer_id = 0;
	};
}