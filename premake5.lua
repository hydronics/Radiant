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
IncludeDir["spdlog"]  = "Radiant/thirdparty/spdlog/include"
IncludeDir["glfw"]    = "Radiant/thirdparty/glfw/include"
IncludeDir["glad"]    = "Radiant/thirdparty/glad/include"
IncludeDir["imgui"]   = "Radiant/thirdparty/imgui"
IncludeDir["glm"]     = "Radiant/thirdparty/glm"

include "Radiant/thirdparty/glfw"
include "Radiant/thirdparty/glad"
include "Radiant/thirdparty/imgui"

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
		"%{IncludeDir.glm}/glm/**.hpp",
		"%{IncludeDir.glm}/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/src/Radiant",
		"%{IncludeDir.spdlog}",
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
		"Radiant/src",
		
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
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
