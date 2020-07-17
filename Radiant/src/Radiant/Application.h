#pragma once

#include "Window.h"
#include "Core.h"

#include "Events/RadiantEvents.h"
#include "LayerStack/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers.h"

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

	private:
		std::unique_ptr<RadiantWindow> m_window;
		ImGuiLayer* m_imgui_layer;
		bool m_running = true;

		LayerStack m_layer_stack;

		unsigned int m_vertex_array;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexBuffer> m_vertex_buffer;
		std::unique_ptr<IndexBuffer>  m_index_buffer;

	private:
		static Application* s_application;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
