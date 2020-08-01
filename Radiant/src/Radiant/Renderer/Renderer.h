#pragma once

#include "RenderCmd.h"
#include "OrthoCamera.h"
#include "Shader.h"

namespace Radiant {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void WindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		static void SubmitDraw(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 view_projection_matrix;
		};

		static SceneData* s_scene_data;
	};

}
