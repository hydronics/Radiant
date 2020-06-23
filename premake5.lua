workspace "Radiant"
	configurations
	{
		"Debug",
		"Release",
		"Production"
	}

	architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/thirdparty/spdlog/include/"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"RD_BUILD_DLL",
			"RD_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RD_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
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
		symbols "on"

	filter "configurations:Release"
		defines "RD_RELEASE"
		optimize "on"

	filter "configurations:Production"
		defines "RD_PRODUCTION"
		optimize "on"
