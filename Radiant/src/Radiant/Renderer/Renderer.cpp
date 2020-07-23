#include "rdpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Radiant {
	Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData;


	void Renderer::Init()
	{
		RenderCmd::Init();
	}

	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCmd::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthoCamera& camera)
	{
		s_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SubmitDraw(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4 transform)
	{
		shader->Bind();

		auto gl_shader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		gl_shader->UploadUniformMat4("u_view_projection", s_scene_data->view_projection_matrix);
		gl_shader->UploadUniformMat4("u_model_transform", transform);

		vertex_array->Bind();
		RenderCmd::DrawIndexed(vertex_array);
	}

}
