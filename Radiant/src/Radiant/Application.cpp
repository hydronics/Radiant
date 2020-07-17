#include "rdpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <Glad/glad.h>
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

		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

		float verts[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{

	}


	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(0.3f, 0.4f, 0.8f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
