#pragma once

#include "Radiant/LayerStack/Layer.h"
#include "Radiant/Events/RadiantEvents.h"

namespace Radiant {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
				
	private:
		float m_time = 0.0f;
	};

}
