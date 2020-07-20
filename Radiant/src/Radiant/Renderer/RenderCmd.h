#pragma once

#include <glad/glad.h>

#include "RendererAPI.h"

namespace Radiant {

	class RenderCmd
	{
	public:
		inline static void Clear()
		{
			s_renderer_api->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_renderer_api->SetClearColor(color);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array)
		{
			s_renderer_api->DrawIndexed(vertex_array);
		}

	private:
		static RendererAPI* s_renderer_api;
	};

}