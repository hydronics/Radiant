#pragma once

#include <string>

namespace Radiant {

	class Shader {
	public:
		Shader(const std::string& vertex_shader_file, const std::string& pixel_shader_file);
		~Shader() {}

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_renderer_id;
	};
}