#include "rdpch.h"
#include "imgui.h"
#include "Platform/opengl/Radiant_imgui_opengl3_impl.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Core.h"
#include "Application.h"
#include "ImGuiLayer.h"
#include "Radiant/Events/Event.h"


namespace Radiant {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		// Sets the framerate for the input, defaults to 60fps first frame
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		//////////////////////////////////////////////////////

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Displays ImGui's demo window always.
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseMoveEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrollEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<MouseClickEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnMouseClickEvent));
		dispatcher.Dispatch<MouseReleaseEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnMouseReleaseEvent));
		dispatcher.Dispatch<KeyboardPressEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnKeyboardPressEvent));
		dispatcher.Dispatch<KeyboardReleaseEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnKeyboardReleaseEvent));
		dispatcher.Dispatch<KeyboardTypedEvent>(RD_BIND_EVENT_FN(ImGuiLayer::OnKeyboardTypedEvent));
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(evt.GetX(), evt.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += evt.GetXOffset();
		io.MouseWheel += evt.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnMouseClickEvent(MouseClickEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[evt.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseReleaseEvent(MouseReleaseEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[evt.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyboardPressEvent(KeyboardPressEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[evt.GetKeycode()] = 1;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyboardReleaseEvent(KeyboardReleaseEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[evt.GetKeycode()] = 0;

		return false;
	}

	bool ImGuiLayer::OnKeyboardTypedEvent(KeyboardTypedEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		int c = evt.GetKeycode();
		if (c > 0 && c < 0x10000)
		{
			io.AddInputCharacter((unsigned short)c);
		}

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& evt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(evt.GetWidth(), evt.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, evt.GetWidth(), evt.GetHeight());

		return false;
	}

}