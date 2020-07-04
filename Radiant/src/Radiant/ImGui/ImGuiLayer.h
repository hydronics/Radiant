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
		// Callback handlers for all the various events we care about


		//  WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		//	AppTick, AppUpdate, AppRender,	
		//	KeyPress, KeyRelease,
		//	MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll

		void OnMouseMoveEvent(MouseMoveEvent& evt);
		void OnMouseScrollEvent(MouseScrollEvent& evt);
		void OnMouseClickEvent(MouseMoveEvent& evt);
		void OnMouseReleaseEvent(MouseMoveEvent& evt);

		void OnKeyboardPressEvent(KeyPressEvent& evt);
		void OnKeyboardReleaseEvent(KeyReleaseEvent& evt);
		//void OnKeyboardTypedEvent(KeyTypedEvent& evt);

		void OnWindowCloseEvent(WindowCloseEvent& evt);
		void OnWindowResizeEvent(WindowResizeEvent& evt);
		void OnWindowMoveEvent(WindowMoveEvent& evt);
		
	private:
		float m_time = 0.0f;
	};

}
