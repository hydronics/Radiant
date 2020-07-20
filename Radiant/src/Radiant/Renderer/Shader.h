#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Radiant {

	class Shader {
	public:
		Shader(const std::string& vertex_shader_file, const std::string& pixel_shader_file);
		~Shader() {}

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix_data);

	private:
		uint32_t m_renderer_id = 0;
	};
}