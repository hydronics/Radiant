#pragma once

#include <glm/glm.hpp>

namespace Radiant {	

	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& Translation, glm::vec3& Rotation, glm::vec3& Scale);

	} // Math

} // Radiant