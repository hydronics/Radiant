#include <Radiant.h>

class ExampleLayer : public Radiant::Layer
{
public:
	ExampleLayer() : Layer()
	{
	}

	virtual void OnUpdate() override
	{
		if (Radiant::Input::IsKeyPressed(RD_KEY_TAB))
		{
			RD_CLIENT_TRACE("Tab key is pressed!");
		}
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnAttach() override
	{

	}

	virtual void OnEvent(Radiant::Event& e) override
	{
	}

};

class Sandbox : public Radiant::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Radiant::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
