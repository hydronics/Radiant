#pragma once

#include <string>
#include <functional>

#include "Events/Event.h"

namespace Radiant {

	// Window Properties
	struct WindowProps {
		WindowProps(const std::string& n = "Radiant Engine",
			unsigned int w = 1280,
			unsigned int h = 720)
			: name(n), width(w), height(h) {}

		std::string name;
		unsigned int width;
		unsigned int height;
	};

	// Window Interface
	class RadiantWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~RadiantWindow() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetVSync(bool vsync) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		virtual void SetEventCallback(EventCallbackFn EvtCallback) = 0;

		static RadiantWindow* Create(const WindowProps& props = WindowProps());
	};
}
