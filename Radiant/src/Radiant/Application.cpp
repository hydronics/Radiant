#include "rdpch.h"

#include "Application.h"
#include "Log.h"

#include "Events/ApplicationEvents.h"

#include "GLFW/glfw3.h"

namespace Radiant {

	Application::Application() {
		m_window = std::unique_ptr<RadiantWindow>(RadiantWindow::Create());
	}

	Application::~Application() {

	}

	void Application::Run() {
		while (m_running)
		{
			glClearColor(0.3, 0.4, 0.8, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}

}
