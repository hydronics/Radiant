#pragma once

#include "Radiant/Core/Layer.h"
#include "Radiant/Events/RadiantEvents.h"

namespace Radiant {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
				
	private:
		float m_time = 0.0f;
	};

}
