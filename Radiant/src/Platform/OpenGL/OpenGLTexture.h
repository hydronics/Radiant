#pragma once

#include "Radiant/Renderer/Texture.h"

namespace Radiant {

	class OpenGLTexture2d : public Texture2d
	{
	public:
		OpenGLTexture2d(const std::string& filepath);
		virtual ~OpenGLTexture2d();

		inline uint32_t GetWidth() const override { return m_width; }
		uint32_t GetHeight() const override { return m_height; }

		void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_filepath;
		uint32_t m_width, m_height;
		uint32_t m_renderer_id;
	};
}
