#pragma once

#include "Radiant/Renderer/Camera.h"

namespace Radiant {

	class SceneCamera : public Camera 
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float near_clip, float far_clip);
		void SetViewportDimensions(uint32_t width, uint32_t height);

		const float GetOrthoSize() const { return m_ortho_size; }
		void SetOrthoSize(float size) { m_ortho_size = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float m_ortho_size = 10.0f;
		float m_ortho_near = -1.0f,  m_ortho_far = 1.0f;

		float m_aspect_ratio = 1.0f;
	};

}
