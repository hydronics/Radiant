#include <Radiant.h>

class TestLayer : public Radiant::Layer
{
public:
	TestLayer() 
		: Layer("test") {}

	void OnEvent(Radiant::Event& e) override
	{
		RD_CLIENT_TRACE("Layer received event");
	}

	void OnUpdate() override
	{
		RD_CLIENT_INFO("TestLayer OnUpdate()");
	}
};



class Sandbox : public Radiant::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new Sandbox();
}
