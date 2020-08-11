#pragma once

#include <glm/glm.hpp>

namespace Radiant {

	// This represents a simple camera project, as we want to use the ECS so it will already have transform.
	class Camera
	{
	public:
		Camera() = default;

	private:
		glm::mat4 m_projection{ 1.0f };
	};

}
