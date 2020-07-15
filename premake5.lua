workspace "Radiant"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Production"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"]    = "Radiant/thirdparty/glfw/include"
IncludeDir["glad"]    = "Radiant/thirdparty/glad/include"
IncludeDir["imgui"]   = "Radiant/thirdparty/imgui"
IncludeDir["glm"]     = "Radiant/thirdparty/glm"

include "Radiant/thirdparty/glfw"
include "Radiant/thirdparty/glad"
include "Radiant/thirdparty/imgui"

project "Radiant"
	location "Radiant"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rdpch.h"
	pchsource "Radiant/src/rdpch.cpp"

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"RD_ENABLE_ASSERTS"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.glm}/glm/**.hpp",
		"%{IncludeDir.glm}/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/src/Radiant",
		"%{prj.name}/thirdparty/spdlog/include",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"glfw",
		"glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
		}

	filter "configurations:Debug"
		defines "RD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
		runtime "Release"
		optimize "on"

		
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Radiant/src",
		"Radiant/thirdparty/spdlog/include",
		"Radiant/thirdparty"
	}

	links
	{
		"Radiant"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

		defines
		{
			"RD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
		runtime "Release"
		optimize "on"
