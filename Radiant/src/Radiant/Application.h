#pragma once

#include "Window.h"
#include "Core.h"

namespace Radiant {

	class RADIANT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<RadiantWindow> m_window;
		bool m_running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
