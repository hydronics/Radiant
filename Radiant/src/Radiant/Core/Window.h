#pragma once

#include <string>
#include <functional>

#include "Radiant/Core/Timestep.h"
#include "Radiant/Events/Event.h"

namespace Radiant {

	// Window Properties
	struct WindowProps {
		WindowProps(const std::string& n = "Radiant Engine",
			uint32_t w = 1920,
			uint32_t h = 1080)
			: name(n), width(w), height(h) {}

		std::string name;
		uint32_t width;
		uint32_t height;
	};

	// Window Interface
	class RadiantWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~RadiantWindow() {}

		virtual void OnUpdate(Timestep timestep) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetVSync(bool vsync) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		virtual void SetEventCallback(EventCallbackFn EvtCallback) = 0;

		virtual void* GetNativeWindow() const = 0;


		static RadiantWindow* Create(const WindowProps& props = WindowProps());
	};
}
