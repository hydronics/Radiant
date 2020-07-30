#pragma once

#include <glm/glm.hpp>

#include "OrthoCamera.h"
#include "Texture.h"

namespace Radiant {

	struct SceneData {
		glm::mat4 view_projection_matrix;
	};

	class Renderer2d
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const SceneData& scene);
		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		// vec3 API allows z-coord of position to set directly the z-order layer.
		// vec2 API will put all quads on z=0 position.
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2d>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture2d>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec2& rotation, const Ref<Texture2d>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec2& rotation, const Ref<Texture2d>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec2& rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec2& rotation, const glm::vec4& color);

	};
}
