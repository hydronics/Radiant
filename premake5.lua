workspace "Radiant"
	configurations
	{
		"Debug",
		"Release",
		"Production"
	}

	architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"] = "Radiant/thirdparty/glfw/include"
IncludeDir["glad"] = "Radiant/thirdparty/glad/include"

include "Radiant/thirdparty/glfw"
include "Radiant/thirdparty/glad"

project "Radiant"
	location "Radiant"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rdpch.h"
	pchsource "Radiant/src/rdpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/thirdparty/spdlog/include/",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}"
	}

	links
	{
		"glfw",
		"glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"RD_BUILD_DLL",
			"RD_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RD_DEBUG;RD_ENABLE_ASSERTS"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
		buildoptions "/MD"
		optimize "on"

		
project "Sandbox"

	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Radiant/thirdparty/spdlog/include",
		"Radiant/src"
	}

	links
	{
		"Radiant"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"RD_PLATFORM_WINDOWS"		
		}

	filter "configurations:Debug"
		defines "RD_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
		buildoptions "/MD"
		optimize "on"
