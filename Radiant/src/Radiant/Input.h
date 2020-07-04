#pragma once

#include "Core.h"

namespace Radiant {

	class RADIANT_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return s_impl->IsKeyPressed(keycode); };
		inline static bool IsMouseButtonClicked(int button) { return s_impl->IsMouseClicked(button); };
		inline static bool GetMouseX(int keycode) { return s_impl->GetMouseX(); };
		inline static bool GetMouseY(int keycode) { return s_impl->GetMouseY(); };

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonClickedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_impl;

	};

}