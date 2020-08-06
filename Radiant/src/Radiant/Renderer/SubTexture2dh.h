#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Radiant {

	class SubTexture2d
	{
	public:
		SubTexture2d(const Ref<Texture2d>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2d> GetTexture() const { return m_texture; }
		const glm::vec2* GetTextureCoords() const { return m_tex_coords; }

		static Ref<SubTexture2d> CreateFromCoords(const Ref<Texture2d>& texture, const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size);

	private:
		Ref<Texture2d> m_texture;
		glm::vec2 m_tex_coords[4];
	};

}
