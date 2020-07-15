#include <Radiant.h>

class ExampleLayer : public Radiant::Layer
{
public:
	ExampleLayer() : Layer()
	{
	}

	virtual void OnUpdate() override
	{
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
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
