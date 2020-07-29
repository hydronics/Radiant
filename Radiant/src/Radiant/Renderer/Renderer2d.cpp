#include "rdpch.h"
#include "Renderer2d.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCmd.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Radiant {

	struct RendererStorage {
		Ref<VertexArray> flat_vertex_array;
		Ref<Shader> flat_color_shader;
		Ref<Shader> texture_shader;
	};

	static RendererStorage* s_storage;

	void Renderer2d::Init()
	{
		s_storage = new RendererStorage();

		s_storage->flat_vertex_array = Radiant::VertexArray::Create();
		float square_verts[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Radiant::Ref<Radiant::VertexBuffer> square_vb = Radiant::VertexBuffer::Create(sizeof(square_verts), square_verts);
		square_vb->SetLayout({
			{ Radiant::ShaderDataType::Float3, "a_position" },
			{ Radiant::ShaderDataType::Float2, "a_tex_coords" },
			});
		s_storage->flat_vertex_array->AddVertexBuffer(square_vb);

		uint32_t square_indices[] = { 0, 1, 2,  2, 3, 0 };
		Radiant::Ref<Radiant::IndexBuffer> square_ib = Radiant::IndexBuffer::Create(sizeof(square_indices) / sizeof(uint32_t), square_indices);
		s_storage->flat_vertex_array->SetIndexBuffer(square_ib);

		s_storage->flat_color_shader = Radiant::Shader::Create("assets/shaders/flat_shader.glsl");

		s_storage->texture_shader = Radiant::Shader::Create("assets/shaders/texture.glsl");
		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetInt("u_texture", 0);
	}

	void Renderer2d::Shutdown()
	{
		delete s_storage;
	}

	void Renderer2d::BeginScene(const SceneData& scene)
	{
		s_storage->flat_color_shader->Bind();
		s_storage->flat_color_shader->SetMat4("u_view_projection", scene.view_projection_matrix);

		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetMat4("u_view_projection", scene.view_projection_matrix);
	}

	void Renderer2d::BeginScene(const OrthoCamera& camera)
	{
		s_storage->flat_color_shader->Bind();
		s_storage->flat_color_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());
	}

	void Renderer2d::EndScene()
	{
	}

	void Renderer2d::DrawQuad(const glm::vec4& color, const glm::vec3& position, const glm::vec2& scale, const glm::vec2& rotation)
	{
		s_storage->flat_color_shader->Bind();

		s_storage->flat_color_shader->SetFloat4("u_color", color);
		s_storage->flat_color_shader->SetMat4("u_model_transform", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f}));

		s_storage->flat_vertex_array->Bind();
		RenderCmd::DrawIndexed(s_storage->flat_vertex_array);
	}

	void Renderer2d::DrawQuad(const glm::vec4& color, const glm::vec2& position, const glm::vec2& scale, const glm::vec2& rotation)
	{
		DrawQuad(color, { position.x, position.y, 0.0f }, scale, rotation);
	}

	void Renderer2d::DrawQuad(const Ref<Texture2d>& texture, const glm::vec3& position, const glm::vec2& scale)
	{
		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetMat4("u_model_transform", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f }));

		texture->Bind(0);

		s_storage->flat_vertex_array->Bind();
		RenderCmd::DrawIndexed(s_storage->flat_vertex_array);
	}

	void Renderer2d::DrawQuad(const Ref<Texture2d>& texture, const glm::vec2& position, const glm::vec2& scale)
	{
		DrawQuad(texture, { position.x, position.y, 0.0f }, scale);
	}

}
