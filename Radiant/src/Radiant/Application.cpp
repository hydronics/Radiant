#include "rdpch.h"

#include "Application.h"
#include "Log.h"

#include <Glad/glad.h>

namespace Radiant { 
	Application* Application::s_application = nullptr;

	Application::Application()
	{
		RD_CORE_ASSERT(!s_application, "Application already exists!  Cannot create another instance!");
		s_application = this;

		m_window = std::unique_ptr<RadiantWindow>(RadiantWindow::Create());
		m_window->SetEventCallback(RD_BIND_EVENT_FN(Application::OnEvent));

		unsigned int id;
		glGenVertexArrays(1, &id);
	}

	Application::~Application()
	{

	}


	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(0.3, 0.4, 0.8, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto iter : m_layer_stack)
			{
				iter->OnUpdate();
			}
			m_window->OnUpdate();
		}
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RD_BIND_EVENT_FN(Application::OnWindowClose));

		RD_CORE_TRACE("{0}", e.ToString());

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
