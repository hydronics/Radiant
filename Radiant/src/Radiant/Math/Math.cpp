#include "rdpch.h"

#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Radiant {

	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& Translation, glm::vec3& Rotation, glm::vec3& Scale)
		{
			// implementation from glm::decompose in matrix_decompose.inl
			using namespace glm;
			using T = float;

			// Setup
			mat4 LocalMatrix(transform);
						
			// Normalize matrix
			if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
				return false;

			// isolate perspective
			if (epsilonEqual(LocalMatrix[0][3], static_cast<float>(0), epsilon<T>()) ||
				epsilonEqual(LocalMatrix[1][3], static_cast<float>(0), epsilon<T>()) ||
				epsilonEqual(LocalMatrix[2][3], static_cast<float>(0), epsilon<T>()))
			{
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}

			// Transl;ation
			Translation = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);
			///////////////////////////////////////////////////////////


			// Scale
			vec3 Row[3], Pdum3;

			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			Scale.x = length(Row[0]);
			Row[0] = detail::scale(Row[0], static_cast<T>(1));

			Scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));

			Scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));
			///////////////////////////////////////////////////////////


			// Rotation
			Rotation.y = asin(-Row[0][2]);
			if (cos(Rotation.y) != 0)
			{
				Rotation.x = atan2(Row[1][2], Row[2][2]);
				Rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else
			{
				Rotation.x = atan2(-Row[1][0], Row[1][1]);
				Rotation.z = 0;
			}
			///////////////////////////////////////////////////////////

			return true;
		}

	} // Math
} // Radiant