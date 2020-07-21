#include <Radiant.h>

#include <glm/gtc/type_ptr.hpp>

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
		std::shared_ptr<Radiant::VertexBuffer> vertex_buffer;
		vertex_buffer.reset(Radiant::VertexBuffer::Create(sizeof(verts), verts));

		Radiant::BufferLayout layout = {
			{Radiant::ShaderDataType::Float3, "a_position"},
			{Radiant::ShaderDataType::Float4, "a_color"}
		};

		vertex_buffer->SetLayout(layout);
		m_vertex_array->AddVertexBuffer(vertex_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Radiant::IndexBuffer> index_buffer;
		index_buffer.reset(Radiant::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_vertex_array->SetIndexBuffer(index_buffer);


		// ************************************************************ //


		m_square_va.reset(Radiant::VertexArray::Create());
		float square_verts[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.8f, 0.1f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.8f, 0.5f, 0.2f, 0.0f
		};
		std::shared_ptr<Radiant::VertexBuffer> square_vb;
		square_vb.reset(Radiant::VertexBuffer::Create(sizeof(square_verts), square_verts));
		square_vb->SetLayout(layout);
		m_square_va->AddVertexBuffer(square_vb);

		uint32_t square_indices[] = { 0, 1, 2,  2, 3, 0 };
		std::shared_ptr<Radiant::IndexBuffer> square_ib;
		square_ib.reset(Radiant::IndexBuffer::Create(sizeof(square_indices) / sizeof(uint32_t), square_indices));
		m_square_va->SetIndexBuffer(square_ib);



		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_transform;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_view_projection * u_model_transform * vec4(a_position, 1.0f);
			}
		)";

		std::string pixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;

			uniform vec4 u_color;

			void main()
			{
				color = u_color;
			}
		)";

		m_shader.reset(new Radiant::Shader(vertexSrc, pixelSrc));

	}

	virtual void OnDetach() override
	{
	}

	virtual void OnAttach() override
	{
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
		glm::vec4 red(0.7f, 0.2f, 0.1f, 1.0f);
		glm::vec4 blue(0.1f, 0.2f, 0.7f, 1.0f);

		for (int i = 0; i < 25; ++i)
		{
			for (int j = 0; j < 25; ++j)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (i % 2 == 0)
				{
					m_shader->UploadUniformVec4("u_color", red);
				}
				else {
					m_shader->UploadUniformVec4("u_color", blue);
				}
				Radiant::Renderer::SubmitDraw(m_shader, m_square_va, transform);
			}
		}

		Radiant::Renderer::EndScene();
	}

	virtual void OnEvent(Radiant::Event& e) override
	{
	}

private:
	std::shared_ptr<Radiant::Shader> m_shader;
	std::shared_ptr<Radiant::VertexArray>  m_vertex_array;
	std::shared_ptr<Radiant::VertexArray> m_square_va;

	Radiant::OrthoCamera m_camera;
	glm::vec3 m_camera_pos = { 0.0f, 0.0f, 0.0f };
	float m_camera_rotation = 0.0f;
	glm::vec2 m_camera_speeds = { 5.0f, 90.0f }; // { translation, rotation }
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
