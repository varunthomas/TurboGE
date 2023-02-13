workspace "TurboGE"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TurboGE/vendor/GLFW/include"
IncludeDir["Glad"] = "TurboGE/vendor/Glad/include"
IncludeDir["Imgui"] = "TurboGE/vendor/imgui"

group "Dependencies"
	include "TurboGE/vendor/GLFW"
	include "TurboGE/vendor/Glad"
	include "TurboGE/vendor/imgui"
	
group ""

project "TurboGE"
	location "TurboGE"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tgepch.h"
	pchsource "TurboGE/src/tgepch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Imgui}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"TGE_DLL_BUILD",
			"TGE_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "TGE_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TurboGE/vendor/spdlog/include",
		"TurboGE/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"TurboGE"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"TGE_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "TGE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "TGE_DIST"
		optimize "On"