#include "rdpch.h"
#include "Radiant/Utilities/PlatformUtils.h"
#include "Radiant/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Radiant {

	std::string FileDialogs::OpenFile(const char* fileFilter)
	{
		OPENFILENAMEA openDialog;
		CHAR fileName[260] = { 0 };
		ZeroMemory(&openDialog, sizeof(OPENFILENAMEA));
		openDialog.lStructSize = sizeof(OPENFILENAMEA);
		openDialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openDialog.lpstrFile = fileName;
		openDialog.nMaxFile = sizeof(fileName);
		openDialog.lpstrFilter = fileFilter;
		openDialog.nFilterIndex = 1;
		openDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&openDialog) == TRUE)
		{
			return openDialog.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* fileFilter)
	{
		OPENFILENAMEA openDialog;
		CHAR fileName[260] = { 0 };
		ZeroMemory(&openDialog, sizeof(OPENFILENAMEA));
		openDialog.lStructSize = sizeof(OPENFILENAMEA);
		openDialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openDialog.lpstrFile = fileName;
		openDialog.nMaxFile = sizeof(fileName);
		openDialog.lpstrFilter = fileFilter;
		openDialog.nFilterIndex = 1;
		openDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&openDialog) == TRUE)
		{
			return openDialog.lpstrFile;
		}

		return std::string();
	}

}
