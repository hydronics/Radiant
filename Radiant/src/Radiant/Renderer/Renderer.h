#pragma once

namespace Radiant {
	
	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		DirectX = 2,
		Vulkan = 3,
		Metal = 4
	};

	class Renderer {
	public:
		inline static RendererAPI GetRendererAPI() { return s_renderer_api; }

	private:
		static RendererAPI s_renderer_api;
	};

}
