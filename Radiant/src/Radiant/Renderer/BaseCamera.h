#pragma once

#include <glm/glm.hpp>

namespace Radiant {

	// This represents a simple camera project, as we want to use the ECS so it will already have transform.
	class BaseCamera
	{
	public:
		BaseCamera() = default;
		BaseCamera(const glm::mat4& proj)
			: m_projection(proj) {}

		virtual ~BaseCamera() = default;

		const glm::mat4& GetProjection() const { return m_projection; }

	protected:
		glm::mat4 m_projection{ 1.0f };
	};

}
