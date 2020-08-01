#include "rdpch.h"
#include <glad/glad.h>

#include "OpenGLRendererAPI.h"

namespace Radiant {
		
	void OpenGLRendererAPI::Init()
	{
		RD_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		glViewport(x, y, w, h);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
	{
		uint32_t count = !index_count ? vertex_array->GetIndexBuffer()->GetCount() : index_count;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
