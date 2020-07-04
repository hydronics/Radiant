#include <Radiant.h>

class Sandbox : public Radiant::Application
{
public:
	Sandbox()
	{
		PushLayer(new Radiant::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
