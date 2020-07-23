#include <Radiant.h>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

class ExampleLayer : public Radiant::Layer
{
public:
	ExampleLayer() : Layer(), m_camera_controller(1280.0f / 720.0f, true)
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

		uint32_t indices[] = { 0, 1, 2 };
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

		m_shader_library.Load("assets/shaders/texture.glsl");
		m_shader = Radiant::Shader::Create("flat_shader", vertexSrc, pixelSrc);

		m_texture = Radiant::Texture2d::Create("assets/textures/Checkerboard.png");
		m_cherno_logo_texture = Radiant::Texture2d::Create("assets/textures/ChernoLogo.png");
		auto texture_shader = m_shader_library.Get("texture");
		std::dynamic_pointer_cast<Radiant::OpenGLShader>(texture_shader)->Bind();
		std::dynamic_pointer_cast<Radiant::OpenGLShader>(texture_shader)->UploadUniformInt("u_texture", 0);
	}

	virtual void OnUpdate(Radiant::Timestep timestep) override
	{
		float ts = timestep;

		// OnUpdate phase
		m_camera_controller.OnUpdate(timestep);

		// OnRender phase

		Radiant::RenderCmd::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		Radiant::RenderCmd::Clear();

		Radiant::Renderer::BeginScene(m_camera_controller.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Radiant::OpenGLShader>(m_shader)->Bind();
		std::dynamic_pointer_cast<Radiant::OpenGLShader>(m_shader)->UploadUniformFloat3("u_color", m_color);

		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Radiant::Renderer::SubmitDraw(m_shader, m_square_va, transform);
			}
		}
		auto texture_shader = m_shader_library.Get("texture");
		m_texture->Bind();
		Radiant::Renderer::SubmitDraw(texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 1.0f)));
		m_cherno_logo_texture->Bind();
		Radiant::Renderer::SubmitDraw(texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 1.0f)));

		Radiant::Renderer::EndScene();
	}

	virtual void OnEvent(Radiant::Event& e) override
	{
		m_camera_controller.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Radiant Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
		ImGui::End();
	}

private:
	Radiant::ShaderLibrary m_shader_library;

	Radiant::Ref<Radiant::Texture2d> m_texture;
	Radiant::Ref<Radiant::Texture2d> m_cherno_logo_texture;

	Radiant::Ref<Radiant::Shader> m_shader;
	Radiant::Ref<Radiant::VertexArray> m_vertex_array;
	Radiant::Ref<Radiant::VertexArray> m_square_va;

	Radiant::OrthoCameraController m_camera_controller;
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
