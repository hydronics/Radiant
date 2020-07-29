#include "rdpch.h"

#include <glad/glad.h>

#include "stb_image.h"
#include "OpenGLTexture.h"

namespace Radiant {

	OpenGLTexture2d::OpenGLTexture2d(const std::string& filepath)
		: m_filepath(filepath)
	{
		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* texture = stbi_load(filepath.c_str(), &w, &h, &channels, 0);

		RD_CORE_ASSERT("Texture: {0} - LOAD FAILED", filepath);

		m_width = w;
		m_height = h;

		GLenum internal_format = 0;
		GLenum data_format = 0;

		if (channels == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (channels == 3)
		{
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		RD_CORE_ASSERT(internal_format & data_format, "Format not supported!!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, internal_format, m_width, m_height);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, texture);

		stbi_image_free(texture);
	}

	OpenGLTexture2d::~OpenGLTexture2d()
	{
		glDeleteTextures(1, &m_renderer_id);
	}

	void OpenGLTexture2d::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

}