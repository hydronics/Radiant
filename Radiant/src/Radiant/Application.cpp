#include "rdpch.h"

#include "Application.h"
#include "Radiant/Events/ApplicationEvents.h"
#include "Radiant/Log.h"

namespace Radiant {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowMovedEvent ev(100,100);

		RD_CLIENT_TRACE(ev.ToString());

		while (true);
	}

}
