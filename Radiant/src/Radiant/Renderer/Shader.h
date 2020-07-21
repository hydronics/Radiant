#pragma once

#include <string>

namespace Radiant {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertex_shader_file, const std::string& pixel_shader_file);

	private:
		uint32_t m_renderer_id = 0;
	};
}