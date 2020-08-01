#pragma once

#include "Radiant/Renderer/RendererAPI.h"

namespace Radiant {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0) override;
	};

}
