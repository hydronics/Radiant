#include "rdpch.h"
#include "SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Radiant {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float near_clip, float far_clip)
	{
		m_ortho_size = size;
		m_ortho_near = near_clip;
		m_ortho_far = far_clip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportDimensions(uint32_t width, uint32_t height)
	{
		m_aspect_ratio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float left = -m_ortho_size * m_aspect_ratio * 0.5f;
		float right = m_ortho_size * m_aspect_ratio * 0.5f;
		float bottom = -m_ortho_size * 0.5f;
		float top = m_ortho_size * 0.5f;

		m_projection = glm::ortho(left, right, bottom, top, m_ortho_near, m_ortho_far);
	}

}