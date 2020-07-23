#pragma once

#include "Window.h"
#include "Core.h"
#include "Radiant/Core/Timestep.h"

#include "Radiant/Core/LayerStack.h"
#include "Radiant/ImGui/ImGuiLayer.h"
#include "Radiant/Events/RadiantEvents.h"

namespace Radiant {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);

		inline RadiantWindow& GetWindow() { return *m_window; }
		static Application& Get() { return *s_application; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<RadiantWindow> m_window;
		ImGuiLayer* m_imgui_layer;
		LayerStack m_layer_stack;
		float m_prev_frame_time = 0.0f;

		bool m_running = true;
		bool m_minimized = false;
		bool m_is_resizing = false;

	private:
		static Application* s_application;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
