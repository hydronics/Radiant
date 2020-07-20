#include "rdpch.h"
#include <glad/glad.h>

#include "OpenGLRendererAPI.h"

namespace Radiant {
		
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array)
	{
		glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

}