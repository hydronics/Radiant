#include <Radiant.h>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

class ExampleLayer : public Radiant::Layer
{
public:
	ExampleLayer() : Layer(), m_camera(-3.2f, 3.2f, -1.8f, 1.8f)
	{
		m_vertex_array.reset(Radiant::VertexArray::Create());
		float verts[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.8f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.1f, 0.0f,
		};
		Radiant::Ref<Radiant::VertexBuffer> vertex_buffer;
		vertex_buffer.reset(Radiant::VertexBuffer::Create(sizeof(verts), verts));

		vertex_buffer->SetLayout({
			{Radiant::ShaderDataType::Float3, "a_position"}
		});
		m_vertex_array->AddVertexBuffer(vertex_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Radiant::Ref<Radiant::IndexBuffer> index_buffer;
		index_buffer.reset(Radiant::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_vertex_array->SetIndexBuffer(index_buffer);


		// ************************************************************ //


		m_square_va.reset(Radiant::VertexArray::Create());
		float square_verts[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Radiant::Ref<Radiant::VertexBuffer> square_vb;
		square_vb.reset(Radiant::VertexBuffer::Create(sizeof(square_verts), square_verts));
		square_vb->SetLayout({
			{ Radiant::ShaderDataType::Float3, "a_position" },
			{ Radiant::ShaderDataType::Float2, "a_tex_coords" }
		});
		m_square_va->AddVertexBuffer(square_vb);

		uint32_t square_indices[] = { 0, 1, 2,  2, 3, 0 };
		Radiant::Ref<Radiant::IndexBuffer> square_ib;
		square_ib.reset(Radiant::IndexBuffer::Create(sizeof(square_indices) / sizeof(uint32_t), square_indices));
		m_square_va->SetIndexBuffer(square_ib);


		std::string texShaderVertSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 0) in vec2 a_tex_coords;

			out vec2 v_tex_coords;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_transform;

			void main()
			{
				v_tex_coords = a_tex_coords;
				gl_Position = u_view_projection * u_model_transform * vec4(a_position, 1.0f);
			}
		)";

		std::string texShaderPixSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec2 v_tex_coords;

			void main()
			{
				color = vec4(v_tex_coords, 0.0, 1.0);
			}
		)";
		m_texture_shader.reset(Radiant::Shader::Create(texShaderVertSrc, texShaderPixSrc));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			out vec3 v_position;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_transform;

			void main()
			{
				v_position = a_position;
				gl_Position = u_view_projection * u_model_transform * vec4(a_position, 1.0f);
			}
		)";

		std::string pixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;

			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		m_shader.reset(Radiant::Shader::Create(vertexSrc, pixelSrc));
	}

	virtual void OnUpdate(Radiant::Timestep timestep) override
	{
		float ts = timestep;

		// Update camera based on any input for this frame
		if (Radiant::Input::IsKeyPressed(RD_KEY_D))
		{
			m_camera_pos.x += m_camera_speeds.x * ts;
		}
		else if (Radiant::Input::IsKeyPressed(RD_KEY_A))
		{
			m_camera_pos.x -= m_camera_speeds.x * ts;
		}
		if (Radiant::Input::IsKeyPressed(RD_KEY_W))
		{
			m_camera_pos.y += m_camera_speeds.x * ts;
		}
		else if (Radiant::Input::IsKeyPressed(RD_KEY_S))
		{
			m_camera_pos.y -= m_camera_speeds.x * ts;
		}

		m_camera.SetPosition(m_camera_pos);
		m_camera.SetRotation(m_camera_rotation);

		Radiant::RenderCmd::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		Radiant::RenderCmd::Clear();

		Radiant::Renderer::BeginScene(m_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto gl_shader = std::dynamic_pointer_cast<Radiant::OpenGLShader>(m_shader);
		gl_shader->Bind();
		gl_shader->UploadUniformFloat3("u_color", m_color);

		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Radiant::Renderer::SubmitDraw(m_shader, m_square_va, transform);
			}
		}
		m_texture_shader->Bind();
		gl_shader->UploadUniformFloat2("u_tex_coords", m_color);

		Radiant::Renderer::SubmitDraw(m_texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 1.0f)));

		Radiant::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Radiant Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
		ImGui::End();
	}

private:
	Radiant::Ref<Radiant::Shader> m_shader;
	Radiant::Ref<Radiant::Shader> m_texture_shader;
	Radiant::Ref<Radiant::VertexArray> m_vertex_array;
	Radiant::Ref<Radiant::VertexArray> m_square_va;

	Radiant::OrthoCamera m_camera;
	glm::vec3 m_camera_pos = { 0.0f, 0.0f, 0.0f };
	float m_camera_rotation = 0.0f;
	glm::vec2 m_camera_speeds = { 5.0f, 90.0f }; // { translation, rotation }
	glm::vec3 m_color = { 0.1f, 0.2f, 0.7f };
};

class Sandbox : public Radiant::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
