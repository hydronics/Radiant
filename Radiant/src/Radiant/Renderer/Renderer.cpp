#include "rdpch.h"
#include "Renderer.h"

namespace Radiant {
	Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData;


	void Renderer::BeginScene(const OrthoCamera& camera)
	{
		s_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SubmitDraw(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array, const glm::mat4 transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_view_projection", s_scene_data->view_projection_matrix);
		shader->UploadUniformMat4("u_model_transform", transform);

		vertex_array->Bind();
		RenderCmd::DrawIndexed(vertex_array);
	}

}
