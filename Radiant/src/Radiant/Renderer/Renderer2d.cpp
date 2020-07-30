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
		Ref<Shader> texture_shader;
		Ref<Texture2d> white_texture;
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

		s_storage->white_texture = Texture2d::Create(1, 1);
		uint32_t white_color_data = 0xffffffff;
		s_storage->white_texture->SetData(&white_color_data, sizeof(white_color_data));

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
		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetMat4("u_view_projection", scene.view_projection_matrix);
	}

	void Renderer2d::BeginScene(const OrthoCamera& camera)
	{
		s_storage->texture_shader->Bind();
		s_storage->texture_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());
	}

	void Renderer2d::EndScene()
	{
	}

	void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		s_storage->texture_shader->SetFloat4("u_color", color);
		s_storage->texture_shader->SetMat4("u_model_transform", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f}));

		s_storage->white_texture->Bind(0);

		s_storage->flat_vertex_array->Bind();
		RenderCmd::DrawIndexed(s_storage->flat_vertex_array);
	}

	void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, scale, color);
	}

	void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2d>& texture)
	{
		s_storage->texture_shader->SetFloat4("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });
		s_storage->texture_shader->SetMat4("u_model_transform", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f }));

		texture->Bind(0);

		s_storage->flat_vertex_array->Bind();
		RenderCmd::DrawIndexed(s_storage->flat_vertex_array);
	}

	void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture2d>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, scale, texture);
	}

}
