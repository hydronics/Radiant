#pragma once

#include "RendererAPI.h"

namespace Radiant {

	class RenderCmd
	{
	public:
		inline static void Init()
		{
			s_renderer_api->Init();
		}
		inline static void Clear()
		{
			s_renderer_api->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_renderer_api->SetClearColor(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0)
		{
			s_renderer_api->DrawIndexed(vertex_array, index_count);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_renderer_api->SetViewport(0, 0, width, height);
		}

	private:
		static RendererAPI* s_renderer_api;
	};

}