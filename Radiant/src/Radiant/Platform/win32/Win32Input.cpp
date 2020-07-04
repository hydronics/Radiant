#pragma once
#include "rdpch.h"

#include "Win32Input.h"
#include "Radiant/Application.h"

#include <GLFW/glfw3.h>

namespace Radiant {

	Input* Input::s_impl = new Win32Input();
	
	bool Win32Input::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Win32Input::IsMouseButtonClickedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
		
	}

	std::pair<float, float> Win32Input::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float Win32Input::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();

		return (float)x;
	}

	float Win32Input::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();

		return (float)y;
	}

}