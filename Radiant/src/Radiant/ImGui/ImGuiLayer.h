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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_block_events = block; }
	private:
		bool m_block_events = true;
		float m_time = 0.0f;
	};

}
