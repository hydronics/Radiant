#include <Radiant.h>
#include "ShadesmarEditorLayer.h"

// ---Entry Point--------------------
#include "Radiant/Core/EntryPoint.h"
// ---Entry Point--------------------

namespace Radiant {

	class ShadesmarEditorApp : public Radiant::Application
	{
	public:
		ShadesmarEditorApp()
			: Application("Shadesmar Editor")
		{
			PushLayer(new ShadesmarEditorLayer());
		}

		~ShadesmarEditorApp()
		{
		}
	};

	Radiant::Application* Radiant::CreateApplication() {
		return new ShadesmarEditorApp();
	}

}
