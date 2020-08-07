#pragma once
#include "rdpch.h"
#include "Radiant/Core/Input.h"
#include "Radiant/Core/InputUtils.h"
#include "Radiant/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Radiant {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		
		auto state = glfwGetKey(window, Radiant_To_GLFW_Keycode(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonClicked(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
		
	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePos();

		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePos();

		return (float)y;
	}

}
