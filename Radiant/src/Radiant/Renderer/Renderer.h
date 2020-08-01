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

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 view_projection_matrix;
		};

		static SceneData* s_scene_data;
	};

}
