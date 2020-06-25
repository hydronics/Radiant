#pragma once

#include "Window.h"
#include "Core.h"

#include "Events/RadiantEvents.h"
#include "LayerStack/LayerStack.h"

namespace Radiant {

	class RADIANT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<RadiantWindow> m_window;
		bool m_running = true;

		LayerStack m_layer_stack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
