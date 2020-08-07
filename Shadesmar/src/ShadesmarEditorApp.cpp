#include <Radiant.h>
#include "ShadesmarEditorLayer.h"

// ---Entry Point--------------------
#include "Radiant/Core/EntryPoint.h"
// ---Entry Point--------------------


class ShadesmarEditorApp : public Radiant::Application
{
public:
	ShadesmarEditorApp()
		: Application("Shadesmar Editor")
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new ShadesmarEditorLayer());
	}

	~ShadesmarEditorApp()
	{
	}
};

Radiant::Application* Radiant::CreateApplication() {
	return new ShadesmarEditorApp();
}
