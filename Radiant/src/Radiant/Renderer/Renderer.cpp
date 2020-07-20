#include "rdpch.h"
#include "Renderer.h"

namespace Radiant {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SubmitDraw(const std::shared_ptr<VertexArray>& vertex_array)
	{
		vertex_array->Bind();
		RenderCmd::DrawIndexed(vertex_array);
	}

}
