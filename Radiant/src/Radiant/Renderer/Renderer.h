#pragma once

#include "RenderCmd.h"

namespace Radiant {


	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void SubmitDraw(const std::shared_ptr<VertexArray>& vertex_array);

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }
	};

}
