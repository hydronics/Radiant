#include "rdpch.h"
#include "Renderer2d.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCmd.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Radiant {

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		float tex_index;
		float tiling_factor;
	};

	struct RendererData2d {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Query GPU for this, RenderCapabilities class/struct

		Ref<VertexArray> quads_vertex_array;
		Ref<VertexBuffer> quads_vertex_buffer;
		Ref<Shader> texture_shader;
		Ref<Texture2d> white_texture;
		std::array<Ref<Texture2d>, MaxTextureSlots> texture_slots;
		uint32_t texture_slot_index = 1; // slot 0 == white texture, always bound to slot 0


		// TODO: Wrap this in struct
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		glm::vec4 QuadVertexPositions[4] = {};

		Renderer2d::Statistics Stats;
	};

	static RendererData2d s_data;

	void Renderer2d::Init()
	{
		RD_PROFILE_FUNCTION();

		s_data.quads_vertex_array = VertexArray::Create();

		s_data.quads_vertex_buffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
		s_data.quads_vertex_buffer->SetLayout({
			{ Radiant::ShaderDataType::Float3, "a_position" },
			{ Radiant::ShaderDataType::Float4, "a_color" },
			{ Radiant::ShaderDataType::Float2, "a_tex_coords" },
			{ Radiant::ShaderDataType::Float, "a_tex_index" },
			{ Radiant::ShaderDataType::Float, "a_tiling_factor" }
			});
		s_data.quads_vertex_array->AddVertexBuffer(s_data.quads_vertex_buffer);

		s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];


		uint32_t* quad_indices = new uint32_t[s_data.MaxIndices];

		// Add all the indices here since they can be pre-computed.
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;

			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;

			offset += 4;
		}

		Radiant::Ref<Radiant::IndexBuffer> square_ib = Radiant::IndexBuffer::Create(s_data.MaxIndices, quad_indices);
		s_data.quads_vertex_array->SetIndexBuffer(square_ib);

		delete [] quad_indices;

		s_data.white_texture = Texture2d::Create(1, 1);
		uint32_t white_color_data = 0xffffffff;
		s_data.white_texture->SetData(&white_color_data, sizeof(white_color_data));

		int samplers[s_data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_data.MaxTextureSlots; ++i)
		{
			samplers[i] = i;
		}

		s_data.texture_shader = Radiant::Shader::Create("assets/shaders/texture.glsl");
		s_data.texture_shader->Bind();
		s_data.texture_shader->SetIntArray("u_textures", samplers, s_data.MaxTextureSlots);

		s_data.texture_slots[0] = s_data.white_texture;

		s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2d::Shutdown()
	{
		RD_PROFILE_FUNCTION();
	}

	void Renderer2d::BeginScene(const SceneData& scene)
	{
		RD_PROFILE_FUNCTION();

		s_data.texture_shader->Bind();
		s_data.texture_shader->SetMat4("u_view_projection", scene.view_projection_matrix);

		ResetForScene();
	}

	void Renderer2d::BeginScene(const OrthoCamera& camera)
	{
		RD_PROFILE_FUNCTION();

		s_data.texture_shader->Bind();
		s_data.texture_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

		ResetForScene();
	}

	void Renderer2d::BeginScene(const RDCamera& camera, const glm::mat4& transform)
	{
		RD_PROFILE_FUNCTION();

		glm::mat4 view_proj = camera.GetProjection() * glm::inverse(transform);

		s_data.texture_shader->Bind();
		s_data.texture_shader->SetMat4("u_view_projection", view_proj);

		ResetForScene();

	}

	void Renderer2d::EndScene()
	{
		RD_PROFILE_FUNCTION();

		uint32_t data_size = (uint32_t)((uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase);
		s_data.quads_vertex_buffer->SetData(s_data.QuadVertexBufferBase, data_size);

		Flush();
	}

	void Renderer2d::Flush()
	{
		for (uint32_t i = 0; i < s_data.texture_slot_index; ++i)
		{
			s_data.texture_slots[i]->Bind(i);
		}

		RenderCmd::DrawIndexed(s_data.quads_vertex_array, s_data.QuadIndexCount);

		s_data.Stats.DrawCalls++;
	}

	void Renderer2d::ResetForScene()
	{
		s_data.QuadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

		s_data.texture_slot_index = 1;
	}

	void Renderer2d::ResetForNewBatch()
	{
		EndScene();
		ResetForScene();
	}

	void Renderer2d::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		RD_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= RendererData2d::MaxIndices)
		{
			ResetForNewBatch();
		}

		const float texture_index = 0.0f;
		const float tiling_factor = 1.0f;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;
	}

	void Renderer2d::DrawQuad(const glm::mat4& transform, const Ref<Texture2d>& texture, const float tiling_factor /*= 1.0f*/)
	{
		RD_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= RendererData2d::MaxIndices)
		{
			ResetForNewBatch();
		}

		// Base color is white when rendering a textured quad
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		// 
		float texture_index = 0.0f;
		for (uint32_t i = 1; i < s_data.texture_slot_index; ++i)
		{
			if (*s_data.texture_slots[i].get() == *texture.get())
			{
				texture_index = (float)i;
				break;
			}
		}

		if (texture_index == 0.0f)
		{
			texture_index = (float)s_data.texture_slot_index;
			s_data.texture_slots[s_data.texture_slot_index] = texture;
			s_data.texture_slot_index++;
		}

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->color = color;
		s_data.QuadVertexBufferPtr->tex_coord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->tex_index = texture_index;
		s_data.QuadVertexBufferPtr->tiling_factor = tiling_factor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;
	}

	void Renderer2d::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2d>& subtexture, const float tiling_factor /*= 1.0f*/)
	{
		DrawQuad(transform, subtexture->GetTexture(), tiling_factor);
	}

	void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture2d>& texture, const float tiling_factor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, texture, tiling_factor);
	}

	void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<Texture2d>& texture, const float tiling_factor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, texture, tiling_factor);
	}

	void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<SubTexture2d>& subtexture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, subtexture, tiling_factor);
	}

	void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const Ref<SubTexture2d>& subtexture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, subtexture, tiling_factor);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<Texture2d>& texture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, texture, tiling_factor);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<Texture2d>& texture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, texture, tiling_factor);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const Ref<SubTexture2d>& subtexture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, subtexture, tiling_factor);
	}

	void Renderer2d::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const Ref<SubTexture2d>& subtexture, const float tiling_factor /*= 1.0f*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawQuad(transform, subtexture, tiling_factor);
	}

	void Renderer2d::ResetStats()
	{
		memset(&s_data.Stats, 0, sizeof(Statistics));
	}

	Radiant::Renderer2d::Statistics Renderer2d::GetStats()
	{
		return s_data.Stats;
	}

}
