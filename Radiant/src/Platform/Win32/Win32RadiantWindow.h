#pragma once

#include "Radiant/Window.h"
#include "Radiant/Renderer/GraphicsContext.h"
#include "glfw/glfw3.h"


namespace Radiant {

	class Win32RadiantWindow : public RadiantWindow
	{
	public:
		Win32RadiantWindow(const WindowProps& props);
		virtual ~Win32RadiantWindow();

		virtual void OnUpdate(Timestep timestep) override;

		inline virtual unsigned int GetWidth() const override;
		inline virtual unsigned int GetHeight() const override;

		virtual void SetVSync(bool vsync) override;
		virtual bool IsVSyncEnabled() const override;

		inline void SetEventCallback(EventCallbackFn EvtCallback) override { m_window_data.event_callback = EvtCallback; }
		inline virtual void* GetNativeWindow() const override { return m_glfw_window; }

	private:
		// Initialization & Shutdown
		void InitWindow(const WindowProps& props);
		void ShutdownWindow();

	private:
		GLFWwindow* m_glfw_window;
		GraphicsContext* m_context;

		struct WinDat {
			WindowProps props;
			bool vsync_enabled = true;
			RadiantWindow::EventCallbackFn event_callback;
		};

		WinDat m_window_data;
	};
}
