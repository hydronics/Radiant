#include "rdpch.h"

#include "OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Radiant {

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float n, float f) :
		m_view(1.0f), m_projection(glm::orthoLH(left, right, bottom, top, n, f)), m_view_projection(1.0f)
	{
		m_view_projection = m_projection * m_view;
	}

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top, float n /*= -1.0f*/, float f /*= 1.0f*/)
	{
		m_projection = glm::orthoLH(left, right, bottom, top, n, f);
		
		m_view_projection = m_projection * m_view;
	}

	void OrthoCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1), m_position) *
							  glm::rotate(glm::mat4(1), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_view = glm::inverse(transform);
		m_view_projection = m_projection * m_view;
	}

}
