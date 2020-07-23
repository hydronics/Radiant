#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Radiant {

	class RendererAPI
	{
	public:
		enum class API {
			None = 0,
			OpenGL = 1,
			DirectX = 2,
			Vulkan = 3,
			Metal = 4
		};

	public:
		virtual void Init() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertex_array) = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

		inline static API GetAPI() { return s_renderer_api; }

	private:
		static API s_renderer_api;

	};

}
