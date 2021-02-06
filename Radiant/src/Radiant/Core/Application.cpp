#include "rdpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include "Radiant/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Radiant { 
	Application* Application::s_application = nullptr;

	Application::Application(const std::string& name)
	{
		RD_PROFILE_FUNCTION();

		RD_CORE_ASSERT(!s_application, "Application already exists!  Cannot create another instance!");
		s_application = this;

		WindowProps props{
			name,
			1920, 1080
		};

		m_window = Scope<RadiantWindow>(RadiantWindow::Create(props));
		m_window->SetEventCallback(RD_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imgui_layer = new ImGuiLayer;
		PushOverlay(m_imgui_layer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		RD_PROFILE_FUNCTION();

		while (m_running)
		{
			RD_PROFILE_SCOPE("Application RunLoop");

			// Check for escape to close the application
			if (Input::IsKeyPressed(RD_KEY_ESCAPE))
			{
				m_running = false;
				continue;
			}

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_prev_frame_time;
			m_prev_frame_time = time;

			if (!m_minimized)
			{
				{
					RD_PROFILE_SCOPE("LayerStack OnUpdate()");
					for (auto layer : m_layer_stack)
					{
						layer->OnUpdate(timestep);
					}
				}

				{
					RD_PROFILE_SCOPE("LayerStack OnImGuiRender()");
					m_imgui_layer->Begin();
					for (auto layer : m_layer_stack)
					{
						layer->OnImGuiRender();
					}
					m_imgui_layer->End();
				}
			}

			m_window->OnUpdate(timestep);
		}
	}


	void Application::OnEvent(Event& e)
	{
		RD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RD_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(RD_BIND_EVENT_FN(Application::OnWindowResize));
		
		for (auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->OnEvent(e);
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

	void Application::Close()
	{
		m_running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RD_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		m_minimized = false;

		Renderer::WindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
