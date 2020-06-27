#pragma once

#include "Radiant/LayerStack/Layer.h"

namespace Radiant {

	class RADIANT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;

	private:
		float m_time = 0.0f;
	};

}
