#pragma once

#include "Radiant/LayerStack/Layer.h"
#include "Radiant/Events/RadiantEvents.h"

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
		// Callback handlers for all the various events we care about


		//  WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		//	AppTick, AppUpdate, AppRender,	
		//	KeyPress, KeyRelease,
		//	MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll

		bool OnMouseMoveEvent(MouseMoveEvent& evt);
		bool OnMouseScrollEvent(MouseScrollEvent& evt);
		bool OnMouseClickEvent(MouseClickEvent& evt);
		bool OnMouseReleaseEvent(MouseReleaseEvent& evt);
		
		bool OnKeyboardPressEvent(KeyboardPressEvent& evt);
		bool OnKeyboardReleaseEvent(KeyboardReleaseEvent& evt);
		bool OnKeyboardTypedEvent(KeyboardTypedEvent& evt);
		
		bool OnWindowResizeEvent(WindowResizeEvent& evt);
		
	private:
		float m_time = 0.0f;
	};

}
