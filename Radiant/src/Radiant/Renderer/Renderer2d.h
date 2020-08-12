#pragma once

#include <glm/glm.hpp>

#include "OrthoCamera.h"
#include "Camera.h"
#include "Texture.h"
#include "SubTexture2dh.h"

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
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2d>& texture, const float tiling_factor = 1.0f);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2d>& subtexture, const float tiling_factor = 1.0f);

		// vec3 API allows z-coord of position to set directly the z-order layer.
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2d>& texture, const float tiling_factor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SubTexture2d>& subtexture, const float tiling_factor = 1.0f);

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<Texture2d>& texture, const float tiling_factor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<SubTexture2d>& subtexture, const float tiling_factor = 1.0f);

		// vec2 API will put all quads on z=0 position.
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture2d>& texture, const float tiling_factor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<SubTexture2d>& subtexture, const float tiling_factor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<Texture2d>& texture, const float tiling_factor = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<SubTexture2d>& subtexture, const float tiling_factor = 1.0f);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void ResetForNewBatch();
		static void ResetForScene();
	};
}
