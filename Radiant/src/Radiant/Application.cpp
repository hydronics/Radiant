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

		m_window = Scope<RadiantWindow>(RadiantWindow::Create());
		m_window->SetEventCallback(RD_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imgui_layer = new ImGuiLayer;
		PushOverlay(m_imgui_layer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_prev_frame_time;
			m_prev_frame_time = time;


			for (auto layer : m_layer_stack)
			{
				layer->OnUpdate(timestep);
			}


			m_imgui_layer->Begin();
			for (auto layer : m_layer_stack)
			{
				layer->OnImGuiRender();
			}
			m_imgui_layer->End();


			m_window->OnUpdate(timestep);
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
