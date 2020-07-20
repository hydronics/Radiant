#include "rdpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include "Radiant/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Radiant { 
	Application* Application::s_application = nullptr;

	Application::Application()
	{
		RD_CORE_ASSERT(!s_application, "Application already exists!  Cannot create another instance!");
		s_application = this;

		m_window = std::unique_ptr<RadiantWindow>(RadiantWindow::Create());
		m_window->SetEventCallback(RD_BIND_EVENT_FN(Application::OnEvent));

		m_imgui_layer = new ImGuiLayer;
		PushOverlay(m_imgui_layer);



		m_vertex_array.reset(VertexArray::Create());
		float verts[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.8f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.1f, 0.0f,
		};
		std::shared_ptr<VertexBuffer> vertex_buffer;
		vertex_buffer.reset(VertexBuffer::Create(sizeof(verts), verts));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float4, "a_color"}
		};

		vertex_buffer->SetLayout(layout);
		m_vertex_array->AddVertexBuffer(vertex_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> index_buffer;
		index_buffer.reset(IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_vertex_array->SetIndexBuffer(index_buffer);


		// ************************************************************ //


		m_square_va.reset(VertexArray::Create());
		float square_verts[] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> square_vb;
		square_vb.reset(VertexBuffer::Create(sizeof(square_verts), square_verts));
		BufferLayout square_layout = {
			{ShaderDataType::Float3, "a_position"}
		};

		square_vb->SetLayout(square_layout);
		m_square_va->AddVertexBuffer(square_vb);

		uint32_t square_indices[] = { 0, 1, 2,  2, 3, 0 };
		std::shared_ptr<IndexBuffer> square_ib;
		square_ib.reset(IndexBuffer::Create(sizeof(square_indices) / sizeof(uint32_t), square_indices));
		m_square_va->SetIndexBuffer(square_ib);



		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0f);
			}
		)";

		std::string pixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;
			
			void main()
			{
				color = v_color;
			}
		)";

		std::string squareVertexShaderSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			void main()
			{
				gl_Position = vec4(a_position, 1.0f);
			}
		)";

		std::string squarePixelShaderSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.2, 0.8, 0.1, 1.0);
			}
		)";

		m_shader.reset(new Shader(vertexSrc, pixelSrc));
		m_square_shader.reset(new Shader(squareVertexShaderSrc, squarePixelShaderSrc));
	}

	Application::~Application()
	{

	}


	void Application::Run()
	{
		while (m_running)
		{
			RenderCmd::SetClearColor({ 0.2f, 0.12f, 0.8f, 1.0f });
			RenderCmd::Clear();

			Renderer::BeginScene();

			m_square_shader->Bind();
			Renderer::SubmitDraw(m_square_va);

			m_shader->Bind();
			Renderer::SubmitDraw(m_vertex_array);

			Renderer::EndScene();

			for (auto layer : m_layer_stack)
			{
				layer->OnUpdate();
			}
			m_imgui_layer->Begin();
			for (auto layer : m_layer_stack)
			{
				layer->OnImGuiRender();
			}
			m_imgui_layer->End();

			// @TODO:  This code will need to be abstracted to the Renderer API in Radiant.  

			m_window->OnUpdate();
		}
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RD_BIND_EVENT_FN(Application::OnWindowClose));
		
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.handled) break;
		}
	}


	void Application::PushOverlay(Layer* overlay)
	{
		m_layer_stack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

}
