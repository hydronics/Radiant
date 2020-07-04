#pragma once

#include "Core.h"

namespace Radiant {

	class RADIANT_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return s_impl->IsKeyPressedImpl(keycode); };
		inline static bool IsMouseButtonClicked(int button) { return s_impl->IsMouseButtonClickedImpl(button); };
		inline static std::pair<float,float> GetMousePos() { return s_impl->GetMousePosImpl(); };
		inline static float GetMouseX() { return s_impl->GetMouseXImpl(); };
		inline static float GetMouseY() { return s_impl->GetMouseYImpl(); };

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonClickedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_impl;

	};

}