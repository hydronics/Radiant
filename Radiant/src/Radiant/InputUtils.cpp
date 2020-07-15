#include "rdpch.h"

#include "InputUtils.h"

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
