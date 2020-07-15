#pragma once

#include "Radiant/Input.h"

namespace Radiant {

	class Win32Input : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonClickedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};


	// Utility functions for converting between different platform keycodes
	int GLFW_To_Radiant_Keycode(int glfwKeyCode)
	{
		// Since we base Radiant keycodes off GLFW keycodes, no conversion necessary
		return glfwKeyCode;

	}

	int Radiant_To_GLFW_Keycode(int radiantKeyCode)
	{
		return radiantKeyCode;
	}

}
