#include "rdpch.h"
#include "RDSceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Radiant {

	RDSceneCamera::RDSceneCamera()
	{
		RecalculateProjection();
	}

	void RDSceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		ProjType = ProjectionType::Orthographic;
		OrthographicSize = size;
		OrthographicNear = nearClip;
		OrthographicFar = farClip;
		RecalculateProjection();
	}

	void RDSceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		ProjType = ProjectionType::Perspective;
		PerspectiveVerticalFov = verticalFov;
		PerspectiveNear = nearClip;
		PerspectiveFar = farClip;
		RecalculateProjection();

	}

	void RDSceneCamera::SetViewportDimensions(uint32_t width, uint32_t height)
	{
		AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void RDSceneCamera::RecalculateProjection()
	{
		if (ProjType == ProjectionType::Orthographic)
		{
			float left = -OrthographicSize * AspectRatio * 0.5f;
			float right = OrthographicSize * AspectRatio * 0.5f;
			float bottom = -OrthographicSize * 0.5f;
			float top = OrthographicSize * 0.5f;

			m_projection = glm::ortho(left, right, bottom, top, OrthographicNear, OrthographicFar);
		}
		else if (ProjType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(PerspectiveVerticalFov, AspectRatio, PerspectiveNear, PerspectiveFar);
		}
	}

}