#pragma once

#include "Radiant/Window.h"
#include "glfw/glfw3.h"


namespace Radiant {

	class Win32RadiantWindow : public RadiantWindow
	{
	public:
		Win32RadiantWindow(const WindowProps& props);
		virtual ~Win32RadiantWindow();

		virtual void OnUpdate() override;

		inline virtual unsigned int GetWidth() const override;
		inline virtual unsigned int GetHeight() const override;

		virtual void SetVSync(bool vsync) override;
		virtual bool IsVSyncEnabled() const override;

		inline virtual void SetEventCallback(EventCallbackFn EvtCallback) override;

	private:
		// Initialization & Shutdown
		void InitWindow(const WindowProps& props);
		void ShutdownWindow();

	private:
		GLFWwindow* m_glfw_window;

		struct WinDat {
			WindowProps props;
			bool vsync_enabled;
			RadiantWindow::EventCallbackFn event_callback;
		};

		WinDat m_window_data;
	};
}