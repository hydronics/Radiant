#include "rdpch.h"
#include "SubTexture2dh.h"

namespace Radiant {

	SubTexture2d::SubTexture2d(const Ref<Texture2d>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_texture(texture)
	{
		m_tex_coords[0] = { min.x, min.y };
		m_tex_coords[1] = { max.x, min.y };
		m_tex_coords[2] = { max.x, max.y };
		m_tex_coords[3] = { min.x, max.y };
	}

	Ref<SubTexture2d> SubTexture2d::CreateFromCoords(const Ref<Texture2d>& texture, const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size = { 1, 1 })
	{
		const glm::vec2 min = { (coords.x * cell_size.x) / texture->GetWidth(), (coords.y * cell_size.y) / texture->GetHeight() };
		const glm::vec2 max = { ((coords.x + sprite_size.x) * cell_size.x) / texture->GetWidth(), ((coords.y + sprite_size.y) * cell_size.y) / texture->GetHeight() };
		return CreateRef<SubTexture2d>(texture, min, max);
	}

}
