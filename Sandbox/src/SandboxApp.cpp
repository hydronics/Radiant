#include <Radiant.h>
#include "Sandbox2d.h"

// ---Entry Point--------------------
#include "Radiant/Core/EntryPoint.h"
// ---Entry Point--------------------


class Sandbox : public Radiant::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2d());
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
