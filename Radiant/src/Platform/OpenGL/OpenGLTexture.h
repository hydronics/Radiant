#pragma once

#include "Radiant/Renderer/Texture.h"
#include <glad/glad.h>

namespace Radiant {

	class OpenGLTexture2d : public Texture2d
	{
	public:
		OpenGLTexture2d(const std::string& filepath);
		OpenGLTexture2d(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2d();

		inline uint32_t GetWidth() const override { return m_width; }
		uint32_t GetHeight() const override { return m_height; }

		void Bind(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& rhs) const override
		{
			return m_renderer_id == ((OpenGLTexture2d&)rhs).m_renderer_id;
		}

	private:
		std::string m_filepath;
		uint32_t m_width, m_height;
		uint32_t m_renderer_id;
		GLenum m_internal_format;
		GLenum m_data_format;
	};
}
